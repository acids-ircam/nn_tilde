import torch
from typing import  List, Dict

class Buffer():
    value: torch.Tensor
    min_samples: int
    max_samples: int
    def __init__(self, tensor: torch.Tensor | None = None, 
                 min_samples: int = -1, 
                 max_samples: int = -1, 
                 sr: int | float | None = -1):
        self.value = torch.tensor(0)
        self.init_value()
        if tensor is not None:
            self.value = tensor
        self.min_samples = min_samples
        self.max_samples = max_samples
        self.sr = -1 if sr is None else int(sr)

    def check_bounds(self, x: torch.Tensor) -> bool:
        is_ok = True
        if self.min_samples != -1:
            is_ok = is_ok and x.shape[-1] >= self.min_samples
        if self.max_samples != -1:
            is_ok = is_ok and x.shape[-1] <= self.max_samples
        return is_ok

    def from_buffer(self, buffer: "Buffer"):
        return self.set_value(buffer.value, sr = buffer.sr)

    def set_value(self, x: torch.Tensor, sr: int | float | None = None) -> int:        
        _has_valid_bounds = self.check_bounds(x)
        if not _has_valid_bounds:
            return -1
        if sr is None:
            self.sr = -1
        else:
            self.sr = int(sr)
        self.value = x.clone()
        return 0

    @property
    def shape(self) -> List[int]:
        if self.has_value: 
            return self.value.shape
        else:
            return torch.Size([])

    @property
    def has_value(self) -> bool:
        return self.value.numel() != 0

    def get_value(self) -> torch.Tensor:
        return self.value

    def init_value(self) -> None:
        self.value = torch.zeros(0, 0, 0)
        
    def to_str(self) -> str:
        if self.has_value:
            out = f"Buffer(min={self.value.min()}, max={self.value.max()}, sr={self.sr}, shape={self.shape})"
        else:
            out = "Buffer(empty)"
        return out


BUFFER_ATTRIBUTES_TYPE = Dict[str, Buffer]