import sys, os
import torch
import pytest
from pathlib import Path

sys.path.append(str(Path(__file__).parent / ".." / ".."))
from python_tools import Module, TYPE_HASH, Buffer
from types import MethodType
from typing import NoReturn
from utils import out_dir, test_name, import_code 


class AttributeFoo(Module):
    def __init__(self):
        super().__init__()
        self.register_attribute("attr_int", 0)
        self.register_attribute("attr_float", 0.)
        self.register_attribute("attr_str", "apple")
        self.register_attribute("attr_bool", False)
        self.register_method("forward", 1, 1, 2, 1, test_method=False)
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor):
        x = torch.zeros(x.shape[:-2] + (2, x.shape[-1]))
        x[..., 0, :] = self.attr_int[0]
        x[..., 1, :] = self.attr_float[0]
        return x


class ListAttributeFoo(Module):
    def __init__(self, n: int, attribute_type: type):
        super().__init__()
        self.n = n
        if attribute_type == torch.Tensor:
            attr = tuple([torch.tensor(i) for i in range(n)])
        else:
            attr = tuple([attribute_type(i) for i in range(n)])
        self.register_attribute("attr", attr)
        self.register_method("forward", 1, 1, n, 1, output_labels=["attribute %d"%i for i in range(len(attr))])
        self.finish()

    def forward(self, x):
        out = torch.zeros(x.shape[:1] + (len(self.attr),) + x.shape[2:])
        for i, val in enumerate(self.attr):
            out[..., i, :] = float(val)
        return out

def _default(attr_hash: int):
    type_hash_r = {v: k for k, v in TYPE_HASH.items()}
    attr_hash = type_hash_r[int(attr_hash)]
    if attr_hash in [bool, int, float, str]: 
        return attr_hash(1)
    elif attr_hash in [torch.Tensor]:
        return torch.tensor(0)
    elif attr_hash in [Buffer]:
        return (torch.tensor(0), 44100)
    else:
        raise TypeError('type not known')


class TensorAttributeFoo(Module): 
    def __init__(self):
        super().__init__()
        self.register_attribute("a", torch.zeros(4))
        self.register_method("forward", 1, 1, 4, 1, test_method=False)
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor):
        out = torch.zeros(x.shape[0], 4, x.shape[2])
        for i in range(4):
            out[:, i] = self.a[0][None, i]
        return out
        

@pytest.mark.parametrize('module_class', [AttributeFoo])
def test_attributes(module_class, out_dir, test_name):
    module = module_class()
    for attr_name in module.get_attributes().value:
        # test getter
        attr_params = getattr(module, f"{attr_name}_params")
        getattr(module, f"get_{attr_name}")()
        # test setter
        getattr(module, f"set_{attr_name}")(_default(attr_params[0]))
    module(torch.zeros(1, 1, 16))
    scripted = torch.jit.script(module)
    torch.jit.save(scripted, out_dir/f"{test_name}.ts")


@pytest.mark.parametrize('n', [1, 4])
@pytest.mark.parametrize('attr_type', [str, bool, int, float])
@pytest.mark.parametrize('module_class', [ListAttributeFoo])
def test_list_attributes(n, attr_type, module_class, out_dir, test_name):
    module = module_class(n, attr_type)
    module.get_attr()
    module.set_attr(*([_default(module.attr_params[0])] * n))
    module(torch.zeros(1, 1, 16))
    scripted = torch.jit.script(module)
    torch.jit.save(scripted, out_dir/f"{test_name}.ts")



@pytest.mark.parametrize('n', [1, 4])
@pytest.mark.parametrize('attr_type', [str, bool, int, float])
@pytest.mark.parametrize('module_class', [ListAttributeFoo])
def test_list_attributes(n, attr_type, module_class, out_dir, test_name):
    module = module_class(n, attr_type)
    module.get_attr()
    module.set_attr(*([_default(module.attr_params[0])] * n))
    module(torch.zeros(1, 1, 16))
    scripted = torch.jit.script(module)
    torch.jit.save(scripted, out_dir/f"{test_name}.ts")
    

@pytest.mark.parametrize('module_class', [TensorAttributeFoo])
def test_tensor_attributes(module_class, out_dir, test_name):
    module = module_class()
    target_attr = torch.Tensor([1,2,3,4])
    module.set_a(torch.Tensor(target_attr))
    out = module.get_a()[0]
    assert out.eq(target_attr).all()
    module(torch.zeros(1, 1, 16))
    scripted = torch.jit.script(module)
    torch.jit.save(scripted, out_dir/f"{test_name}.ts")
