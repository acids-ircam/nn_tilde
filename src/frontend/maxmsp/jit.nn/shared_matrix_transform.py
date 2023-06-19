import hashlib
from collections import OrderedDict
from threading import Thread
from time import sleep, time
from typing import Callable, Optional, Sequence

import torch
import torch.nn as nn
from torch.utils import cpp_extension

torch.set_grad_enabled(False)

shared_tensor_lib = cpp_extension.load(
    "shared_tensor",
    sources=["shared_tensor.cpp"],
    extra_include_paths=["/Users/acaillon/code/shared/boost_1_82_0"],
    verbose=False,
)


class BufferedComputation:

    def __init__(self, batch_size: int,
                 apply_fun: Callable[[torch.Tensor], torch.Tensor]) -> None:
        self.input_buffer = []
        self.output_buffer = []
        self.result = None
        self.apply_fun = apply_fun
        self.batch_size = batch_size
        self.thread = None

    def __call__(self, x: torch.Tensor) -> torch.Tensor:
        self.input_buffer.append(x)

        if len(self.input_buffer) >= self.batch_size:
            input_buffer = torch.stack(self.input_buffer, 0)
            self.input_buffer = []
            if self.thread is not None:
                self.thread.join()
                self.output_buffer.extend(list(self.result))
            self.thread = Thread(target=threaded_forward,
                                 args=(self, input_buffer))
            self.thread.start()

        if len(self.output_buffer) < self.batch_size:
            return torch.zeros_like(x)
        else:
            return self.output_buffer.pop(0)


def threaded_forward(buffered_class: BufferedComputation,
                     input_buffer: torch.Tensor) -> torch.Tensor:
    output_buffer = buffered_class.apply_fun(input_buffer)
    buffered_class.result = output_buffer.cpu()


class SharedMatrixTransform(nn.Module):

    def __init__(
        self,
        rescale: Optional[Sequence[int]] = None,
        batch_size: int = 1,
    ) -> None:
        super().__init__()
        self.cache = OrderedDict()
        self.permute = torch.Tensor([[0, 0, 1], [1, 0, 0], [0, 1, 0]])
        self.rescale = rescale
        if self.rescale is not None:
            self.rescale = nn.Upsample(size=rescale)

        self.buffer = BufferedComputation(batch_size=batch_size,
                                          apply_fun=self.forward)

        self.previous_frame_hash = None

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
        while True:
            start_time = time()
            try:
                stream_name = f"{name}_input"
                input_tensor_ptr = shared_tensor.get_shared_tensor()
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
            if self.previous_frame_hash is None:
                self.previous_frame_hash = tensor_hash
            elif self.previous_frame_hash == tensor_hash:
                pass
            else:
                self.previous_frame_hash = tensor_hash
                output_tensor = self.buffer(input_tensor)

                if output_tensor.shape != input_tensor.shape:
                    raise ValueError(
                        f"Inconsistent shapes, got {input_tensor.shape} -> {output_tensor.shape} "
                    )
                else:
                    try:
                        stream_name = f"{name}_output"
                        if self.rescale is not None:
                            output_tensor = output_tensor.permute(2, 0,
                                                                  1)[None]
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
                            f"Couldn't find shared stream with name {stream_name}"
                        )

            current_dt = time() - start_time

            if current_dt < dt:
                sleep(dt - current_dt)
            else:
                overtime = 1000 * (current_dt - dt)
                print(f"process late by {overtime:.2f}ms" + 10 * " ", end="\r")
