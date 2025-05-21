import inspect
import logging
from typing import Any,  List, Optional, Sequence, Tuple, Union
from types import MethodType

import cached_conv as cc
import torch

from . import TYPE_HASH

from .buffer import Buffer, BUFFER_ATTRIBUTES_TYPE
from . import templates
from .codegen import *


class Module(torch.nn.Module):
    __reserved_attribute_names__ = ['sr']
    def __init__(self, sr: int | None = None) -> None:
        super().__init__()
        self._methods = []
        self._attributes = torch.jit.Attribute([], List[str])
        self._buffer_attributes = torch.jit.Attribute([], List[str])
        self.tmp_file_session = tmp_file_session(self)
        self._ready = False
        self.sr = torch.jit.Attribute(sr, int | None) 

    def register_method(
        self,
        method_name: str,
        in_channels: int,
        in_ratio: int,
        out_channels: int,
        out_ratio: int,
        input_labels: Optional[Sequence[str]] = None,
        output_labels: Optional[Sequence[str]] = None,
        test_method: bool = True,
        test_buffer_size: int = 8192,
    ):
        """Register a class method as usable by nn~.

        The method must take as input and return a single 3D tensor.

        Args:
            method_name: name of the method to register
            in_channels: number of channels of the input tensor
            in_ratio: temporal compression ratio of the input tensor
            in_channels: number of channels of the output tensor
            in_ratio: temporal compression ratio of the output tensor
            input_labels: labels used by max for the inlets
            output_labels: labels used by max for the outlets
            test_method: weither the method is tested during registration or not
            test_buffer_size: duration of the test buffer
        """
        logging.info(f'Registering method "{method_name}"')
        self.register_buffer(
            f'{method_name}_params',
            torch.tensor([
                in_channels,
                in_ratio,
                out_channels,
                out_ratio,
            ]))

        if input_labels is None:
            input_labels = [
                f"(signal) model input {i}" for i in range(in_channels)
            ]
        if len(input_labels) != in_channels:
            raise ValueError(
                (f"Method {method_name}, expected "
                 f"{in_channels} input labels, got {len(input_labels)}"))
        setattr(self, f"{method_name}_input_labels", input_labels)

        if output_labels is None:
            output_labels = [
                f"(signal) model output {i}" for i in range(out_channels)
            ]
        if len(output_labels) != out_channels:
            raise ValueError(
                (f"Method {method_name}, expected "
                 f"{out_channels} output labels, got {len(output_labels)}"))
        setattr(self, f"{method_name}_output_labels", output_labels)

        if test_method:
            logging.info(f"Testing method {method_name} with nn~ API")
            x = torch.zeros(1, in_channels, test_buffer_size // in_ratio)
            y = getattr(self, method_name)(x)

            if len(y.shape) != 3:
                raise ValueError(
                    ("Output tensor must have exactly 3 dimensions, "
                     f"got {len(y.shape)}"))
            if y.shape[0] != 1:
                raise ValueError(
                    f"Expecting single batch output, got {y.shape[0]}")
            if y.shape[1] != out_channels:
                raise ValueError((
                    f"Wrong number of output channels for method \"{method_name}\", "
                    f"expected {out_channels} got {y.shape[1]}"))
            if y.shape[2] != test_buffer_size // out_ratio:
                raise ValueError(
                    (f"Wrong output length for method \"{method_name}\", "
                     f"expected {test_buffer_size//out_ratio} "
                     f"got {y.shape[2]}"))
            if y.dtype != torch.float:
                raise ValueError(f"Output tensor must be of type float, got {y.dtype}")

            if cc.MAX_BATCH_SIZE > 1:
                logging.info(f"Testing method {method_name} with mc.nn~ API")
                x = torch.zeros(4, in_channels, test_buffer_size // in_ratio)
                y = getattr(self, method_name)(x)

                if len(y.shape) != 3:
                    raise ValueError(
                        ("Output tensor must have exactly 3 dimensions, "
                         f"got {len(y.shape)}"))
                if y.shape[0] != 4:
                    raise ValueError(
                        f"Expecting 4 batch output, got {y.shape[0]}")
                if y.shape[1] != out_channels:
                    raise ValueError((
                        f"Wrong number of output channels for method \"{method_name}\", "
                        f"expected {out_channels} got {y.shape[1]}"))
                if y.shape[2] != test_buffer_size // out_ratio:
                    raise ValueError(
                        (f"Wrong output length for method \"{method_name}\", "
                         f"expected {test_buffer_size//out_ratio} "
                         f"got {y.shape[2]}"))
                logging.info((f"Added method \"{method_name}\" "
                              f"tested with buffer size {test_buffer_size}"))
            else:
                logging.info(
                    f"Skipping method {method_name} with mc.nn~ API as cc.MAX_BATCH_SIZE={cc.MAX_BATCH_SIZE}"
                )
        else:
            logging.warn(f"Added method \"{method_name}\" without testing it.")

        self._methods.append(method_name)

    def register_attribute(self, attribute_name: str,
                           values: Union[Any, Tuple[Any]]):
        """Register an attribute visible by nn~.

        Args:
            attribute_name: name of the attribute to register
            values: a default value or tuple of default values
        """
        assert attribute_name not in self.__reserved_attribute_names__, f"attribute_name {attribute_name} is reserved."

        if not isinstance(values, (List, Tuple)):
            values = list([values])
        else:
            values = list(values)

        type_hash = []
        for v in values:
            type_hash.append(TYPE_HASH[type(v)])

        # if not hasattr(self, f"get_{attribute_name}"):
        #     raise AttributeError(f"Getter for {attribute_name} not defined")

        # if not hasattr(self, f"set_{attribute_name}"):
        #     raise AttributeError(f"Setter for {attribute_name} not defined")

        # signature = inspect.signature(getattr(self, f"get_{attribute_name}"))
        # if signature.return_annotation == inspect._empty:
        #     raise TypeError(
        #         f"Output type not defined for getter get_{attribute_name}")

        self.__setattr__(attribute_name, tuple(values))
        self._attributes.value.append(attribute_name)
        for i, v in enumerate(values):
            if isinstance(v, Buffer):
                self._buffer_attributes.value.append(f"{attribute_name}#{i}")
        self.register_buffer(f"{attribute_name}_params", torch.LongTensor(type_hash))

    @torch.jit.export
    def get_methods(self):
        return self._methods

    @torch.jit.export
    def get_attributes(self) -> List[str]:
        return self._attributes

    def export_to_ts(self, path):
        self.eval()
        scripted = torch.jit.script(self)
        scripted.save(path)

    # buffer attributes handling
    @torch.jit.export
    def get_buffer_attributes(self) -> List[str]:
        if torch.jit.is_scripting():
            return self._buffer_attributes
        else:
            return self._buffer_attributes.value

    @torch.jit.export
    def is_buffer_empty(self, buffer_name: str) -> bool:
        if buffer_name not in self.get_buffer_attributes():
            return True
        if torch.jit.is_scripting():
            for k, v in self._buffer_attributes.items():
                if k == buffer_name:
                    return v.has_value
        else:
            for k, v in self._buffer_attributes.value.items():
                if k == buffer_name:
                    return v.has_value
        return True

    @torch.jit.export
    def get_sample_rate(self) -> int: 
        sr = self.sr
        if torch.jit.isinstance(sr, Optional[int]):
            if sr is None:
                return -1
            else:
                return int(sr)
        else:
            if sr.value is None:
                return -1
            else:
                return int(sr.value)

    @torch.jit.export
    def set_sample_rate(self, sample_rate: int | None) -> None:
        """set the operative sampling rate of the module on inference."""
        if sample_rate is not None:
            self.sr = sample_rate

    def __prepare_scriptable__(self):
        if not self._ready:
            self.finish()
        return self

    def finish(self):
        self._build_buffer_attribute_methods()
        self._build_missing_attribute_callbacks()
        self._ready = True
    
    def _build_buffer_attribute_methods(self):
        self.set_buffer_attribute = MethodType(
            method_from_template(
                self.tmp_file_session, 
                templates.buffers.set_buffer_attribute_template(self._buffer_attributes),
                globals(), locals()
            )["set_buffer_attribute"], 
            self)

        self.clear_buffer_attribute = MethodType(
            method_from_template(
                self.tmp_file_session, 
                templates.buffers.clear_buffer_attribute_template(self._buffer_attributes),
                globals(), locals()
                )["clear_buffer_attribute"], 
            self)

        self.is_buffer_empty = MethodType(
            method_from_template(
                self.tmp_file_session, 
                templates.buffers.is_buffer_empty_template(self._buffer_attributes),
                globals(), locals()
            )["is_buffer_empty"], 
            self)

    def _build_missing_attribute_callbacks(self):
        for attr_name in self._attributes.value:
            getter_name = f"get_{attr_name}"
            if not hasattr(self, getter_name):
                getter_cb = MethodType(
                    method_from_template(
                    self.tmp_file_session, 
                    templates.attributes.get_attribute_getter(attr_name, getattr(self, f"{attr_name}_params")),
                    globals(), locals()
                )[getter_name], 
                self)
                setattr(self, getter_name, getter_cb)
            setter_name = f"set_{attr_name}"
            if not hasattr(self, setter_name):
                setter_cb = MethodType(
                    method_from_template(
                    self.tmp_file_session, 
                    templates.attributes.get_attribute_setter(attr_name, getattr(self, f"{attr_name}_params")),
                    globals(), locals()
                )[setter_name], 
                self)
                setattr(self, setter_name, setter_cb)


    def __del__(self):
        if getattr(self, "tmp_file_session", None):
            self.tmp_file_session.close()

        
