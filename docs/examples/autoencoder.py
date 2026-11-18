import torch
import torch.nn as nn
import nn_tilde

RATIO = 256      # one latent frame every 256 samples
LATENT = 8       # latent dimensions


class TinyAutoencoder(nn.Module):
    """Stands for your trained model. Kernel size == stride, so each latent
    frame only depends on its own 256 samples : no context needed between
    two nn~ calls."""

    def __init__(self):
        super().__init__()
        self.encoder = nn.Conv1d(1, LATENT, kernel_size=RATIO, stride=RATIO)
        self.decoder = nn.ConvTranspose1d(LATENT, 1, kernel_size=RATIO, stride=RATIO)


class ScriptedAutoencoder(nn_tilde.Module):
    def __init__(self, model: TinyAutoencoder):
        super().__init__()
        self.model = model
        self.register_attribute("temperature", 1.)
        self.register_method(
            "encode", 1, 1, LATENT, RATIO,
            input_labels=["(signal) audio"],
            output_labels=[f"(signal) latent {i}" for i in range(LATENT)],
        )
        self.register_method(
            "decode", LATENT, RATIO, 1, 1,
            input_labels=[f"(signal) latent {i}" for i in range(LATENT)],
            output_labels=["(signal) audio"],
        )
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) audio"],
            output_labels=["(signal) reconstruction"],
        )
        self.finish()

    @torch.jit.export
    def encode(self, x: torch.Tensor) -> torch.Tensor:
        return self.model.encoder(x)

    @torch.jit.export
    def decode(self, z: torch.Tensor) -> torch.Tensor:
        return torch.tanh(self.model.decoder(z * self.temperature[0]))

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.decode(self.encode(x))


if __name__ == "__main__":
    trained = TinyAutoencoder()   # in practice : load your weights here
    ScriptedAutoencoder(trained.eval()).export_to_ts("autoencoder.ts")
