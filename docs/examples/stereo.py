import torch
import nn_tilde


class StereoTools(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.register_attribute("width", 1.)
        self.register_method(
            "midside", 2, 1, 2, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) mid", "(signal) side"],
        )
        self.register_method(
            "widen", 2, 1, 2, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) left", "(signal) right"],
        )
        self.register_method(
            "mono", 2, 1, 1, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) mono"],
        )
        self.finish()

    @torch.jit.export
    def midside(self, x: torch.Tensor) -> torch.Tensor:
        left, right = x[:, 0], x[:, 1]
        return torch.stack([(left + right) / 2, (left - right) / 2], dim=1)

    @torch.jit.export
    def widen(self, x: torch.Tensor) -> torch.Tensor:
        ms = self.midside(x)
        mid, side = ms[:, 0], ms[:, 1] * self.width[0]
        return torch.stack([mid + side, mid - side], dim=1)

    @torch.jit.export
    def mono(self, x: torch.Tensor) -> torch.Tensor:
        return x.mean(dim=1, keepdim=True)

    # a custom setter: refuse negative widths
    @torch.jit.export
    def set_width(self, width: float) -> int:
        if width < 0:
            return -1
        self.width = (width,)
        return 0


if __name__ == "__main__":
    StereoTools().export_to_ts("stereo.ts")
