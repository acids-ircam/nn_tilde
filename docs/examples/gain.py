import torch
import nn_tilde


class Gain(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        # attributes first: register_method() runs the method once to test it
        self.register_attribute("gain", 1.)
        self.register_method(
            "forward",
            in_channels=1, in_ratio=1,
            out_channels=1, out_ratio=1,
            input_labels=["(signal) input"],
            output_labels=["(signal) amplified input"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # x : (batch, 1, n_samples)
        return x * self.gain[0]


if __name__ == "__main__":
    Gain().export_to_ts("gain.ts")
