import torch
import nn_tilde


class Waveshaper(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        # polynomial coefficients c0 + c1 x + c2 x^2 + ... , any length.
        # In Max, set it from an [array] : "set coefs myarray"
        self.register_attribute("coefs", torch.tensor([0., 1.]))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) input"],
            output_labels=["(signal) shaped output"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        coefs = self.coefs[0]
        out = torch.zeros_like(x)
        power = torch.ones_like(x)
        for i in range(coefs.shape[0]):
            out = out + coefs[i] * power
            power = power * x
        return torch.tanh(out)


if __name__ == "__main__":
    Waveshaper().export_to_ts("waveshaper.ts")
