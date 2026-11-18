import torch
import torch.nn as nn
import cached_conv as cc
import nn_tilde

# Must be called BEFORE building the network : cc.Conv1d then keeps the end of
# each buffer in a cache and uses it as left padding for the next one, so the
# output is identical to processing the whole signal at once (no clicks).
cc.use_cached_conv(True)


class Filter(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            cc.Conv1d(1, 16, 31, padding=cc.get_padding(31, mode="causal")),
            nn.Tanh(),
            cc.Conv1d(16, 1, 31, padding=cc.get_padding(31, mode="causal")),
        )
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) input"],
            output_labels=["(signal) filtered"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.net(x)


if __name__ == "__main__":
    model = Filter().eval()
    # check : two consecutive buffers join without discontinuity.
    # (the very first samples differ from offline processing : before the first
    # call the cache holds the response to silence instead of zeros)
    cc.use_cached_conv(False)
    offline = Filter().eval()
    offline.load_state_dict(model.state_dict(), strict=False)
    x = torch.randn(1, 1, 8192)
    with torch.no_grad():
        model(x[..., :4096])
        second_buffer = model(x[..., 4096:])
        error = (second_buffer - offline(x)[..., 4096:]).abs().max().item()
    print(f"streaming error on second buffer: {error:.1e}")
    model.export_to_ts("streaming_conv.ts")
