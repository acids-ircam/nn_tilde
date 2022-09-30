from typing import Any, Callable, Optional, Sequence, Tuple, Union
import torch
import logging

TYPE_HASH = {bool: 0, int: 1, float: 2, str: 3, torch.Tensor: 4}


class Module(torch.nn.Module):

    def __init__(self) -> None:
        super().__init__()
        self._methods = []
        self._attributes = []

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
            x = torch.zeros(1, in_channels, test_buffer_size // in_ratio)
            y = getattr(self, method_name)(x)

            if len(y.shape) != 3:
                raise ValueError(
                    ("Output tensor must have exactly 3 dimensions, "
                     f"got {len(y.shape)}"))
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
            logging.warn(f"Added method \"{method_name}\" without testing it.")

        self._methods.append(method_name)

    def register_attribute(self, attribute_name: str,
                           values: Union[Any, Tuple[Any]]):
        """Register an attribute visible by nn~.

        Args:
            attribute_name: name of the attribute to register
            values: a default value or tuple of default values
        """
        if not isinstance(values, Tuple):
            values = (values, )

        type_hash = tuple(TYPE_HASH[type(v)] for v in values)

        self.register_buffer(attribute_name, torch.Tensor(values))
        self.register_buffer(f"{attribute_name}_params",
                             torch.Tensor(type_hash))
        self._attributes.append(attribute_name)

    @torch.jit.export
    def get_methods(self):
        return self._methods

    @torch.jit.export
    def get_attributes(self):
        return self._attributes

    def export_to_ts(self, path):
        self.eval()
        scripted = torch.jit.script(self)
        scripted.save(path)
