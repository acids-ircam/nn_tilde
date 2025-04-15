try: 
    import nn_tilde
except ImportError: 
    import os, sys
    sys.path.append(os.path.join(os.path.dirname(__file__) , ".." , ".."))
    import python_tools as nn_tilde

from typing import List, Tuple
import torch


class BufferFoo(nn_tilde.Module):
    buffer: Tuple[nn_tilde.Buffer]
    def __init__(self, test_method: bool = False):
        super().__init__()
        # access to max buffers are registered with nn_tilde.Buffer, that can be optionally 
        # given an optional minimum and/or maximum buffer length.
        self.register_attribute("buf", (nn_tilde.Buffer(None, 64, 2048)))
        self.register_method('loudness', 1, 1, 1, 1, test_method=test_method)
        self.register_method('shape', 1, 1, 2, 1, test_method=test_method)
        self.register_method('get_sr', 1, 1, 1, 1, test_method=test_method)
        self.finish()

    def get_loudness(self, x: torch.Tensor) -> float:
        return x.pow(2).mean().sqrt().item()

    @torch.jit.export
    def loudness(self, x: torch.Tensor):
        buffer = self.buf[0]
        if buffer.has_value:
            loudness = self.get_loudness(buffer.value)
            return torch.full_like(x, fill_value=loudness)
        else:
            return torch.zeros_like(x)

    @torch.jit.export
    def shape(self, x: torch.Tensor):
        is_batched = x.ndim > 2
        if not is_batched:
            x = x[None]
        buffer = self.buf[0]
        if buffer.has_value:
            out = torch.zeros(x.shape[0], 2, x.shape[-1])
            out[:, 0, :] = buffer.value.shape[0]
            out[:, 1, :] = buffer.value.shape[1]
            if not is_batched:
                out = out[0]
        else:
            out = torch.zeros_like(x)
        return out

    @torch.jit.export
    def get_sr(self, x: torch.Tensor):
        buffer = self.buf[0]
        if buffer.has_value:
            if self.buf[0].sr is None:
                sr = -1
            else:
                sr = buffer.sr
            return torch.full_like(x, fill_value=sr)
        else:
            return torch.zeros_like(x)

if __name__ == '__main__':
    # Create your target class
    model = BufferFoo()
    # Export it to a torchscript model
    model.export_to_ts('src/models/demo_buffers.ts')

