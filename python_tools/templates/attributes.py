
import torch
from .. import TYPE_HASH, Buffer

TYPE_HASH_R = {v: k for k, v in TYPE_HASH.items()}

def _get_sig_type(param):
    param = TYPE_HASH_R.get(int(param), None)
    if param in [int, float, bool, str]:
        return param.__name__
    elif param in [torch.Tensor]:
        return "torch.Tensor"
    elif param in [Buffer]:
        return "Tuple[torch.Tensor, int]"
    else:
        raise TypeError('type %s not known'%type(param))

def get_attribute_setter(attribute_name, attribute_params):
    signature_atoms = [f'{attribute_name}{i}: {_get_sig_type(attribute_params[i])}' for i in range(len(attribute_params))]
    signature = ", ".join(signature_atoms)
    setter_atoms = []
    for i in range(len(attribute_params)):
        if attribute_params[i] == TYPE_HASH[Buffer]:
            setter_atoms.append(f'Buffer({attribute_name}{i}[0], sr={attribute_name}{i}[1])')
        else:
            setter_atoms.append(f"{attribute_name}{i}")
    setter = f"self.{attribute_name} = (" + ", ".join(setter_atoms) + ",)"
    template = f"@torch.jit.export\ndef set_{attribute_name}(self, {signature}) -> int:\n\tres=0\n"
    template += f"\t{setter}\n"
    template += "\treturn res"
    return template

def get_attribute_getter(attribute_name, attribute_params):
    def _export_arg(attribute_name, attribute_params, i):
        if attribute_params[i] == TYPE_HASH[Buffer]:
            return 'self.' + attribute_name+'['+str(i)+'].to_str()' 
        else:
            return 'self.' + attribute_name+'['+str(i)+']' 
    template = f"@torch.jit.export\ndef get_{attribute_name}(self):\n"
    template+= f"\treturn {', '.join([_export_arg(attribute_name, attribute_params, i) for i in range(len(attribute_params))])},"
    return template

