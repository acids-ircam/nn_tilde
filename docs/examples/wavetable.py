from typing import Tuple
import torch
import nn_tilde


class Wavetable(nn_tilde.Module):
    table: Tuple[nn_tilde.Buffer]

    def __init__(self):
        super().__init__(sr=44100)
        # a Max buffer~ / Pd array, between 16 and 65536 samples long
        self.register_attribute("table", nn_tilde.Buffer(None, 16, 65536))
        self.register_buffer("phase", torch.zeros(1, 1, 1))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) frequency (Hz)"],
            output_labels=["(signal) wavetable output"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, freq: torch.Tensor) -> torch.Tensor:
        if self.phase.shape[0] != freq.shape[0]:
            self.phase = torch.zeros(freq.shape[0], 1, 1)
        increment = freq / float(self.get_sample_rate())
        phase = self.phase + torch.cumsum(increment, dim=-1)
        self.phase = phase[..., -1:] % 1.0

        table = self.table[0]
        if not table.has_value:
            return torch.zeros_like(freq)
        # buffer value : (channels, samples); read the first channel
        wave = table.value[0]
        index = ((phase % 1.0) * wave.shape[0]).long().clamp(0, wave.shape[0] - 1)
        return wave[index]


if __name__ == "__main__":
    Wavetable().export_to_ts("wavetable.ts")
