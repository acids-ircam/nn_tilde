
import pathlib
import torch
TMP_FILE_OUTPUT = pathlib.Path(__file__).parent / ".tmpfile"
from .buffer import Buffer
TYPE_HASH = {bool: 0, int: 1, float: 2, str: 3, torch.Tensor: 4, Buffer: 5}

from . import templates
from .module import Module