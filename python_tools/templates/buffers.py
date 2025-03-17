

def set_buffer_attribute_template(buffer_names):
    template = "@torch.jit.export\ndef set_buffer_attribute(self, buffer_name: str, buffer: torch.Tensor, sr: int) -> int:\n"
    if len(buffer_names.value) == 0:
        template += "\treturn -1"
        return template
    for b in buffer_names.value:
        buffer_names_parts = b.split('#')
        if len(buffer_names_parts) != 2:
            raise ValueError('Invalid buffer name : '%b)
        attribute_name, buffer_idx = buffer_names_parts
        template += f"\tif (buffer_name == \"{b}\"): return self.{attribute_name}[{buffer_idx}].set_value(buffer, sr)\n"
    template += "\treturn -1"
    return template

def clear_buffer_attribute_template(buffer_names):
    template = "@torch.jit.export\ndef clear_buffer_attribute(self, buffer_name: str) -> None:\n"
    if len(buffer_names.value) == 0: 
        template += "\treturn"
        return template
    for b in buffer_names.value:
        buffer_names_parts = b.split('#')
        if len(buffer_names_parts) != 2:
            raise ValueError('Invalid buffer name : '%b)
        attribute_name, buffer_idx = buffer_names_parts
        template += f"\tif (buffer_name == \"{b}\"): return self.{attribute_name}[{buffer_idx}].init_value()\n"
    return template

def is_buffer_empty_template(buffer_names):
    template = "@torch.jit.export\ndef is_buffer_empty(self, buffer_name: str) -> bool:\n"
    if len(buffer_names.value) == 0:
        template += "\treturn True"
        return template
    for b in buffer_names.value:
        buffer_names_parts = b.split('#')
        if len(buffer_names_parts) != 2:
            raise ValueError('Invalid buffer name : '%b)
        attribute_name, buffer_idx = buffer_names_parts
        template += f"\tif (buffer_name == \"{b}\"): return not self.{attribute_name}[{buffer_idx}].has_value\n"
    template += "\treturn True"
    return template
