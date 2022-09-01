import torch, torch.nn as nn, torchaudio.functional as F
from torchaudio.functional.filtering import _lfilter
from typing import List, Tuple

TYPE_HASH = {
    bool: 0,
    int: 1,
    float: 2,
    str: 3,
    torch.Tensor: 4
}


class AudioUtils(nn.Module):
    def __init__(self):
        super().__init__()
        # a exported method XXX must be registered with XXX_params as buffer.
        # method parameters is a tensor of 4 int values : 
        #   [ number_of_inputs, downsize_input_amount, number_of_outputs, downsize_output_amount ]
        # additional names can be given to label Max inputs / outputs (optional)
        self.register_buffer("thru_params", torch.tensor([1, 1, 1, 1]))
        self.thru_input_labels = ['(signal) input signal']
        self.thru_output_labels = ['(signal) output signal']
        self.register_buffer("invert_params", torch.tensor([1, 1, 1, 1]))
        self.invert_input_labels = ['(signal) input signal']
        self.invert_output_labels = ['(signal) output signal']
        self.register_buffer("add_params", torch.tensor([2, 1, 1, 1]))
        self.add_input_labels = ['(signal) first signal', '(signal) second signal']
        self.add_output_labels = ['(signal) output signal']
        self.register_buffer("saturate_params", torch.tensor([1, 1, 1, 1]))
        self.saturate_input_labels = ['(signal) signal to saturate']
        self.saturate_output_labels = ['(signal) saturated signal']
        self.register_buffer("midside_params", torch.tensor([2, 1, 2, 1]))
        self.midside_input_labels = ['(signal) L channel', '(signal) R channel']
        self.midside_output_labels = ['(signal) Mid channel', '(signal) Side channel']
        self.register_buffer("rms_params", torch.tensor([1, 1, 1, 1024]))
        self.rms_input_labels = ['(signal) signal to monitor']
        self.rms_output_labels = ['(signal) rms value']
        self.register_buffer("polynomial_params", torch.tensor([1,1, 1, 1]))
        self.polynomial_input_labels = ['(signal) signal to distort']
        self.polynomial_output_labels = ['(signal) distorted signal']
        self.register_buffer("fractalize_params", torch.tensor([1, 512, 1, 512]))
        self.fractalize_input_labels = ['(signal) signal to replicate']
        self.fractalize_output_labels = ['(signal) fractalized signal']
        
        # defining attributes
        self.gain_factor  = torch.tensor([1.])
        self.polynomial_factors = torch.tensor([1., 0., 0., 0.])
        self.saturate_mode = "tanh"
        self.invert_signal = False
        self.fractal = torch.tensor([2, 0.0])
        
        # to be set within Max,
        # attributes must also be registered with a given params buffer, defining
        #   the type of each attributes' component
        self.gain_factor_params = torch.tensor([TYPE_HASH[float]])
        self.polynomial_factors_params = torch.tensor([TYPE_HASH[float]]*4)
        self.saturate_mode_params = torch.tensor([TYPE_HASH[str]])
        self.invert_signal_params = torch.tensor([TYPE_HASH[bool]])
        self.fractal_params = torch.tensor([TYPE_HASH[int], TYPE_HASH[float]])

    @torch.jit.export
    def thru(self, x: torch.Tensor):
        return x

    # defining main methods
    @torch.jit.export
    def invert(self, x: torch.Tensor):
        if self.invert_signal:
            return x
        else:
            return -x

    @torch.jit.export
    def add(self, x: torch.Tensor):
        return x.sum(-2, keepdim=True) / 2

    @torch.jit.export
    def fractalize(self, x: torch.Tensor):
        fractal_order = int(self.fractal[0])
        fractal_amount = float(self.fractal[1])
        downsampled_signal = x[..., ::fractal_order]
        #x = x + fractal_amount * torch.cat([downsampled_signal]*fractal_order, -1)
        return x

    @torch.jit.export
    def polynomial(self, x: torch.Tensor):
        out = torch.zeros_like(x)
        for i in range(4):
            out += self.polynomial_factors[i] * x.pow(i+1)
        return out

    @torch.jit.export
    def saturate(self, x: torch.Tensor):
        if self.saturate_mode == "tanh":
            return torch.tanh(x * self.gain_factor[0])
        elif self.saturate_mode == "clip":
            return torch.clamp(x * self.gain_factor[0], -1, 1)
    
    @torch.jit.export
    def midside(self, x: torch.Tensor):
        l, r = x[..., 0, :], x[..., 1, :]
        return torch.stack([(l + r)/2, (l - r)/2], dim=-2)

    @torch.jit.export
    def rms(self, x: torch.Tensor):
        x = x.reshape(x.shape[0], x.shape[1], 1024, -1)
        rms = x.pow(2).sum(-2).sqrt() / x.size(-1)
        return rms

    # methods and attributes can be directly given, bypassing nn~ search 
    @torch.jit.export
    def get_methods(self):
        return ['add', 'fractalize', 'polynomial', 'saturate', 'midside', 'rms', 'invert', 'thru']

    @torch.jit.export
    def get_attributes(self):
        return ['gain_factor', 'polynomial_factors', 'saturate_mode', 'invert_signal', 'fractal']

    # defining attribute getters 
    # WARNING : typing the function's ouptut is mandatory
    @torch.jit.export
    def get_gain_factor(self) -> float:
        return float(self.gain_factor[0])
    @torch.jit.export
    def get_polynomial_factors(self) -> List[float]:
        polynomial_factors: List[float] = []
        for p in self.polynomial_factors:
            polynomial_factors.append(float(p))
        return polynomial_factors
    @torch.jit.export
    def get_saturate_mode(self) -> str:
        return self.saturate_mode
    @torch.jit.export
    def get_invert_signal(self) -> bool:
        return self.invert_signal
    @torch.jit.export
    def get_fractal(self) -> Tuple[int, float]:
        return (int(self.fractal[0]), float(self.fractal[1]))
    
    # defining attribute setter
    # setters must return an error code : 
    # return 0 if the attribute has been adequately set,
    # return -1 if the attribute was wrong. 
    @torch.jit.export
    def set_gain_factor(self, x: float) -> int:
        self.gain_factor[0] = x
        return 0
    @torch.jit.export
    def set_polynomial_factors(self, factor1: float, factor2: float, factor3: float, factor4: float) -> int:
        factors = [factor1, factor2, factor3, factor4]
        self.polynomial_factors = torch.tensor(factors)
        return 0
    @torch.jit.export
    def set_saturate_mode(self, x: str):
        if (x == "tanh") or (x=="clip"):
            self.saturate_mode = x
            return 0
        else:
            return -1
    @torch.jit.export
    def set_invert_signal(self, x: bool):
        self.invert_signal = x
        return 0
    @torch.jit.export
    def set_fractal(self, factor: int, amount: float):
        if factor <= 0:
            return -1
        elif factor % 2 != 0:
            return -1
        self.fractal = torch.tensor([float(factor), amount])
        return 0
        


if __name__ == "__main__":
    model = AudioUtils()
    scripted_model = torch.jit.script(model)
    torch.jit.save(scripted_model, 'src/externals/multieffect.ts')
