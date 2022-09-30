import torch
import logging

logging.basicConfig(level=logging.INFO)


class Module(torch.nn.Module):

    def register_method(
        self,
        method_name: str,
        in_channels: int,
        in_ratio: int,
        out_channels: int,
        out_ratio: int,
        test_method: bool = True,
        test_buffer_size: int = 8192,
    ):
        self.register_buffer(
            f'{method_name}_params',
            torch.tensor([
                in_channels,
                in_ratio,
                out_channels,
                out_ratio,
            ]))

        if test_method:
            x = torch.zeros(1, in_channels, test_buffer_size // in_ratio)
            y = self.__getattribute__(method_name)(x)

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

    def export_to_ts(self, path):
        self.eval()
        scripted = torch.jit.script(self)
        scripted.save(path)
