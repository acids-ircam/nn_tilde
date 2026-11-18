import torch
import nn_tilde

HOP = 512


class Analysis(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.hop = HOP  # TorchScript cannot read Python globals : keep it on self
        # 1 audio channel in, 2 control channels out, one value every HOP samples
        self.register_method(
            "describe", 1, 1, 2, HOP,
            input_labels=["(signal) input"],
            output_labels=["(signal) rms", "(signal) zero-crossing rate"],
        )
        self.finish()

    @torch.jit.export
    def describe(self, x: torch.Tensor) -> torch.Tensor:
        # x : (batch, 1, n)  ->  frames : (batch, 1, n // hop, hop)
        b, c, n = x.shape
        frames = x.reshape(b, c, n // self.hop, self.hop)
        rms = frames.pow(2).mean(-1).sqrt()
        signs = torch.sign(frames)
        zcr = (signs[..., 1:] != signs[..., :-1]).float().mean(-1)
        # output : (batch, 2, n // hop)
        return torch.cat([rms, zcr], dim=1)


if __name__ == "__main__":
    Analysis().export_to_ts("analysis.ts")
