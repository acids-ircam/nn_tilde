from typing import Optional, Sequence
import torch
from shared_matrix_transform import SharedMatrixTransform

import torch.nn as nn
from random import shuffle
import matplotlib.pyplot as plt

torch.set_grad_enabled(True)
plt.ion()


class BW_Noise(SharedMatrixTransform):

    def forward(self, x):
        x = x / 255.
        x = torch.mean(x, -1, keepdim=True).expand_as(x)
        x = x + torch.rand_like(x) / 10
        x = torch.clamp(x, 0, 1)
        x = x * 255
        return x


class Flip(SharedMatrixTransform):

    def forward(self, x: torch.Tensor):
        return x.flip(1)


class Identity(SharedMatrixTransform):
    def __init__(self) -> None:
        super().__init__(rescale=(256, 256))

    def forward(self, x):
        return x




Identity().start("debug", 60)
