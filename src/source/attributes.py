try: 
    import nn_tilde
except ImportError: 
    import os, sys
    sys.path.append(os.path.join(os.path.dirname(__file__) , ".." , ".."))
    import python_tools as nn_tilde

from typing import List
import torch


class AttributeFoo(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self._valid_animals_ = torch.jit.Attribute(["horse", "goose", "chicken", "pig", "dog", "cat"], List[str])
        self.register_attribute("attr_int", 0)
        self.register_attribute("attr_float", 0.)
        self.register_attribute("attr_str", "apple")
        self.register_attribute("attr_enum", "horse")
        self.register_attribute("attr_bool", False)
        self.register_attribute("attr_list", [0, "christophe", 1., True])
        self.register_method("forward", 1, 1, 2, 1, test_method=False)
        self.finish()

    @torch.jit.export
    def set_attr_enum(self, animal: str) -> int:
        # a custom setter can accept only given values of an incoming symbol. 
        # a setter function should return 0 if value is accepted, or -1 if refused.
        if animal not in self._valid_animals_:
            return -1
        self.attr_enum = (animal,)
        return 0

    @torch.jit.export 
    def set_attr_list(self, val1: int, val2: str, val3: float, val4: bool):
        # when defining a custom setter for a list, all values must be unfolded in the function
        # signature (this is a TorchScript constraint).
        self.attr_list = (val1, val2, val3, val4)
        return 0

    @torch.jit.export
    def forward(self, x: torch.Tensor):
        x = torch.zeros(x.shape[:-2] + (2, x.shape[-1]))
        x[..., 0, :] = self.attr_int[0]
        x[..., 1, :] = self.attr_float[0]
        return x


if __name__ == '__main__':
    # Create your target class
    model = AttributeFoo()
    # Export it to a torchscript model
    model.export_to_ts('src/models/demo_attributes.ts')

    