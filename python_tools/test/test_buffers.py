import sys, os
import torch
import pytest
from pathlib import Path

sys.path.append(str(Path(__file__).parent / ".." / ".."))
from python_tools import Module, Buffer
from utils import out_dir, test_name, import_code 
from typing import Tuple


class BufferFoo(Module):
    buffer: Tuple[Buffer]
    def __init__(self, test_method: bool = False):
        super().__init__()
        self.register_attribute("buf", Buffer(None, 64, 2048))
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


@pytest.mark.parametrize('module_class', [BufferFoo])
def test_buffer_attributes(module_class, out_dir, test_name):
    module = module_class()
    module.loudness(torch.randn(1, 1, 16))
    module.shape(torch.randn(1, 1, 16))
    module.get_sr(torch.randn(1, 1, 16))
    module.get_buf()
    module.set_buf((torch.zeros(1, 64), 44100))
    module.set_buffer_attribute("buffer#0", torch.zeros(1, 64), 44100)


@pytest.mark.parametrize('module_class', [BufferFoo])
def test_scripted_buffer_attributes(module_class, out_dir, test_name):
    module = module_class()
    scripted = torch.jit.script(module)
    module.loudness(torch.randn(1, 1, 16))
    module.shape(torch.randn(1, 1, 16))
    module.get_sr(torch.randn(1, 1, 16))
    module.get_buf()
    module.set_buf((torch.zeros(1, 64), 44100))
    module.set_buffer_attribute("buffer#0", torch.zeros(1, 64), 44100)
    module.get_buffer_attributes()
    torch.jit.save(scripted, out_dir/f"{test_name}.ts")
    
