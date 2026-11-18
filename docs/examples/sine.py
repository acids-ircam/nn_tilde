import math
import torch
import nn_tilde


class Sine(nn_tilde.Module):
    def __init__(self):
        super().__init__(sr=44100)  # default rate; nn~ sets the real one at load
        # state kept between two calls : one phase per batch
        self.register_buffer("phase", torch.zeros(1, 1, 1))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) frequency (Hz)"],
            output_labels=["(signal) sine"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, freq: torch.Tensor) -> torch.Tensor:
        # the batch size changes with mc.nn~ : reset the state if needed
        if self.phase.shape[0] != freq.shape[0]:
            self.phase = torch.zeros(freq.shape[0], 1, 1)
        increment = freq / float(self.get_sample_rate())
        phase = self.phase + torch.cumsum(increment, dim=-1)
        self.phase = phase[..., -1:] % 1.0
        return torch.sin(2 * math.pi * phase)


if __name__ == "__main__":
    Sine().export_to_ts("sine.ts")
