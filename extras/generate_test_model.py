import torch, torch.nn as nn
import nn_tilde
from typing import List, Tuple


class AudioUtils(nn_tilde.Module):

    def __init__(self):
        super().__init__()
        # REGISTER ATTRIBUTES
        self.register_attribute('gain_factor', 1.)
        self.register_attribute('polynomial_factors', (1., 0., 0., 0.))
        self.register_attribute('saturate_mode', 'tanh')
        self.register_attribute('invert_signal', False)
        self.register_attribute('fractal', (2, 0.))

        # REGISTER METHODS
        self.register_method(
            'thru',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            input_labels=['(signal) input signal'],
            output_labels=['(signal) output signal'],
        )
        self.register_method(
            'invert',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            input_labels=['(signal) input signal'],
            output_labels=['(signal) output signal'],
        )
        self.register_method(
            'add',
            in_channels=2,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            input_labels=['(signal) first signal', '(signal) second signal'],
            output_labels=['(signal) output signal'],
        )
        self.register_method(
            'saturate',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            input_labels=['(signal) signal to saturate'],
            output_labels=['(signal) saturated signal'],
        )
        self.register_method(
            'midside',
            in_channels=2,
            in_ratio=1,
            out_channels=2,
            out_ratio=1,
            input_labels=['(signal) L channel', '(signal) R channel'],
            output_labels=['(signal) Mid channel', '(signal) Side channel'],
        )
        self.register_method(
            'rms',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1024,
            input_labels=['(signal) signal to monitor'],
            output_labels=['(signal) rms value'],
        )
        self.register_method(
            'polynomial',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            input_labels=['(signal) signal to distort'],
            output_labels=['(signal) distorted signal'],
        )

        self.register_method(
            'fractalize',
            in_channels=1,
            in_ratio=512,
            out_channels=1,
            out_ratio=512,
            input_labels=['(signal) signal to replicate'],
            output_labels=['(signal) fractalized signal'],
        )

    @torch.jit.export
    def thru(self, x: torch.Tensor):
        return x

    # defining main methods
    @torch.jit.export
    def invert(self, x: torch.Tensor):
        if self.invert_signal[0]:
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
        return x

    @torch.jit.export
    def polynomial(self, x: torch.Tensor):
        out = torch.zeros_like(x)
        for i in range(4):
            out += self.polynomial_factors[i] * x.pow(i + 1)
        return out

    @torch.jit.export
    def saturate(self, x: torch.Tensor):
        saturate_mode = self.saturate_mode[0]
        if saturate_mode == 'tanh':
            return torch.tanh(x * self.gain_factor[0])
        elif saturate_mode == 'clip':
            return torch.clamp(x * self.gain_factor[0], -1, 1)

    @torch.jit.export
    def midside(self, x: torch.Tensor):
        l, r = x[..., 0, :], x[..., 1, :]
        return torch.stack([(l + r) / 2, (l - r) / 2], dim=-2)

    @torch.jit.export
    def rms(self, x: torch.Tensor):
        x = x.reshape(x.shape[0], x.shape[1], 1024, -1)
        rms = x.pow(2).sum(-2).sqrt() / x.size(-1)
        return rms

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
        return self.saturate_mode[0]

    @torch.jit.export
    def get_invert_signal(self) -> bool:
        return self.invert_signal[0]

    @torch.jit.export
    def get_fractal(self) -> Tuple[int, float]:
        return (int(self.fractal[0]), float(self.fractal[1]))

    # defining attribute setter
    # setters must return an error code :
    # return 0 if the attribute has been adequately set,
    # return -1 if the attribute was wrong.
    @torch.jit.export
    def set_gain_factor(self, x: float) -> int:
        self.gain_factor= (x,)
        return 0

    @torch.jit.export
    def set_polynomial_factors(self, factor1: float, factor2: float,
                               factor3: float, factor4: float) -> int:
        factors = (factor1, factor2, factor3, factor4)
        self.polynomial_factors = factors
        return 0

    @torch.jit.export
    def set_saturate_mode(self, x: str):
        if (x == 'tanh') or (x == 'clip'):
            self.saturate_mode = (x,)
            return 0
        else:
            return -1

    @torch.jit.export
    def set_invert_signal(self, x: bool):
        self.invert_signal = (x,)
        return 0

    @torch.jit.export
    def set_fractal(self, factor: int, amount: float):
        if factor <= 0:
            return -1
        elif factor % 2 != 0:
            return -1
        self.fractal = ( factor,float(amount))
        return 0


if __name__ == '__main__':
    model = AudioUtils()
    model.export_to_ts('multieffect.ts')
