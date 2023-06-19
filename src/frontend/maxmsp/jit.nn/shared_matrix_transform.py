from time import sleep, time

import torch
import torch.nn as nn
from torch.utils import cpp_extension
from collections import OrderedDict
import hashlib
from typing import Optional, Sequence

torch.set_grad_enabled(False)

shared_tensor_lib = cpp_extension.load(
    "shared_tensor",
    sources=["shared_tensor.cpp"],
    extra_include_paths=["/Users/acaillon/code/shared/boost_1_82_0"],
    verbose=False,
)


class SharedMatrixTransform(nn.Module):

    def __init__(self,
                 cache_len: int = 4,
                 rescale: Optional[Sequence[int]] = None) -> None:
        super().__init__()
        self.cache = OrderedDict()
        self.cache_len = cache_len
        self.permute = torch.Tensor([[0, 0, 1], [1, 0, 0], [0, 1, 0]])
        self.rescale = rescale
        if self.rescale is not None:
            self.rescale = nn.Upsample(size=rescale)

    def start(self, name: str, fps: int) -> None:
        try:
            shared_tensor = shared_tensor_lib.SharedTensor(name)
        except RuntimeError:
            raise Exception(
                f"Could't create shared tensor object with id \"{name}\"")
        dt = 1. / fps
        print(
            f"Starting processing stream '{name}' with '{self.__class__.__name__}' at {fps}fps."
        )
        input_tensor_ptr = shared_tensor.get_shared_tensor()
        while True:
            start_time = time()
            try:
                stream_name = f"{name}_input"
                if self.rescale is not None:
                    original_size = input_tensor_ptr.shape[:2]
                    input_tensor = input_tensor_ptr.permute(2, 0, 1)[None]
                    input_tensor = self.rescale(input_tensor)
                    input_tensor = input_tensor[0].permute(1, 2, 0)
                else:
                    input_tensor = input_tensor_ptr
                input_tensor = input_tensor.clone()
            except RuntimeError:
                raise RuntimeError(
                    f"Couldn't find shared stream with name {stream_name}")
            tensor_hash = hashlib.md5(
                input_tensor.numpy().tobytes()).hexdigest()
            if tensor_hash in self.cache:
                output_tensor = self.cache[tensor_hash]
            else:
                output_tensor = self.forward(input_tensor)
                if len(self.cache) == self.cache_len:
                    self.cache.popitem(last=False)
                self.cache[tensor_hash] = output_tensor

            if output_tensor.shape != input_tensor.shape:
                raise ValueError(
                    f"Inconsistent shapes, got {input_tensor.shape} -> {output_tensor.shape} "
                )
            else:
                try:
                    stream_name = f"{name}_output"
                    if self.rescale is not None:
                        output_tensor = output_tensor.permute(2, 0, 1)[None]
                        output_tensor = nn.functional.interpolate(
                            output_tensor,
                            size=original_size,
                            recompute_scale_factor=self.rescale.
                            recompute_scale_factor,
                        )
                        output_tensor = output_tensor[0].permute(1, 2, 0)

                    shared_tensor.set_shared_tensor(output_tensor)
                except RuntimeError:
                    raise RuntimeError(
                        f"Couldn't find shared stream with name {stream_name}")

            current_dt = time() - start_time

            if current_dt < dt:
                sleep(dt - current_dt)
            else:
                overtime = 1000 * (current_dt - dt)
                print(f"process late by {overtime:.2f}ms" + 10 * " ", end="\r")
