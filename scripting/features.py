#
# NN~ - Scripting library
# features.py : Simple scripting example for waveform-to-float case.
#
# We demonstrate the basic mecanisms for using the nn~ environment.
# In this case, any function from Python can be used to wrap it inside a nn~ model.
#
# ACIDS - IRCAM : Philippe Esling, Axel Chemla--Romeu-Santos, Antoine Caillon
#

from typing import List, Tuple
import librosa
# Pytorch audio operations
import torch
import torchaudio.functional as F
from torchaudio.transforms import Spectrogram
# Import the nn~ library
import nn_tilde
import numpy as np

class AudioFeatures(nn_tilde.Module):

    def __init__(self,
                 nfft=1024,
                 hop_size=256,
                 skip_features=None):
        super().__init__()
        self.nfft = nfft
        self.hop_size = hop_size
        transform = Spectrogram(n_fft=nfft,
                                win_length=nfft,
                                hop_length=hop_size,
                                center=False,
                                normalized=True)
        self.transform = transform
        self.skip_features = skip_features
        # -----------------
        # Register attributes
        # -----------------
        self.register_attribute('sr', 44100)
        self.register_buffer('audio_buffer', torch.zeros((1, 1, nfft - hop_size)))
        # Pre-compute frequency bins
        self.freq = torch.fft.rfftfreq(n = nfft, d = 1.0 / self.sr[0])

        # -----------------
        # Register methods
        # -----------------
        self.register_method(
            'rms',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1024,
            input_labels=['(signal) signal to monitor'],
            output_labels=['(signal) rms value'],
        )

        # REGISTER METHODS
        self.register_method(
            'centroid',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=self.hop_size,
            input_labels=['(signal) signal to monitor'],
            output_labels=['(signal) spectral centroid value'],
        )

        # REGISTER METHODS
        self.register_method(
            'flatness',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=self.hop_size,
            input_labels=['(signal) signal to monitor'],
            output_labels=['(signal) flatness value'],
        )

        # REGISTER METHODS
        self.register_method(
            'bandwidth',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=self.hop_size,
            input_labels=['(signal) signal to monitor'],
            output_labels=['(signal) bandwidth value'],
        )

    def _compute_spectrogram(self, x: torch.Tensor):
        # X : B x hop_size
        if self.audio_buffer.shape[0] != x.shape[0]:
            print("Resizing and resetting buffer - the batch size has changed")
            self.audio_buffer = torch.zeros((x.shape[0], 1, self.nfft - self.hop_size)).to(x)
            self.freq = torch.fft.rfftfreq(n = self.nfft, d = 1.0 / self.sr[0])
        # Using the previous buffer information
        x = torch.cat([self.audio_buffer, x], dim=-1)
        # Compute the transform
        spec = self.transform(x)[:, 0]
        self.audio_buffer = x[..., -(self.nfft - self.hop_size):]
        if self.skip_features is not None:
            spec = spec[:, :self.skip_features]
        return spec

    @torch.jit.export
    def rms(self, x: torch.Tensor):
        x = x.reshape(x.shape[0], x.shape[1], 1024, -1)
        rms = x.pow(2).sum(-2).sqrt() / x.size(-1)
        return rms
    
    @torch.jit.export
    def centroid(self, x: torch.Tensor):
        # Compute the current spectrogram
        spectro = self._compute_spectrogram(x)
        # Compute the center frequencies of each bin
        if self.freq is None:
            self.freq = torch.fft.rfftfreq(n = self.nfft, d = 1.0 / self.sr[0])
        if len(self.freq.shape) == 1:
            self.freq = self.freq[None, :, None].expand_as(spectro)
        # Column-normalize S
        centroid = torch.sum(self.freq * torch.nn.functional.normalize(spectro, p=1.0, dim=-2), dim=-2)
        return centroid[:, None, :]

    @torch.jit.export
    def bandwidth(self, x: torch.Tensor, amin: float = 1e-10, power: float = 2.0, p: float = 2.0):
        # Compute the current spectrogram
        spectro = self._compute_spectrogram(x)
        # Compute the center frequencies of each bin
        if self.freq is None:
            self.freq = torch.fft.rfftfreq(n = self.nfft, d = 1.0 / self.sr[0])
        if len(self.freq.shape) == 1:
            self.freq = self.freq[None, :, None].expand_as(spectro)
        # Normalize spectro
        spectro_normed = torch.nn.functional.normalize(spectro, p=1.0, dim=-2)
        # Compute centroid
        centroid = torch.sum(self.freq * spectro_normed, dim=-2)[:, None, :]
        # Compute the deviation
        deviation = torch.abs(self.freq - centroid)
        # Compute bandwidth
        bandwidth = torch.sum(spectro_normed * deviation**p, dim=-2, keepdim=True) ** (1.0 / p)
        return bandwidth

    @torch.jit.export
    def flatness(self, x: torch.Tensor, amin: float = 1e-10, power: float = 2.0):
        # Compute the current spectrogram
        spectro = self._compute_spectrogram(x)
        S_thresh = torch.maximum(spectro**power, torch.zeros(1) + amin)
        gmean = torch.exp(torch.mean(torch.log(S_thresh), dim=-2, keepdim=True))
        amean = torch.mean(S_thresh, dim=-2, keepdim=True)
        flatness = gmean / amean
        print(flatness.shape)
        return flatness

    # defining attribute getters
    # WARNING : typing the function's ouptut is mandatory
    @torch.jit.export
    def get_sr(self) -> int:
        return int(self.sr[0])

    # defining attribute setter
    # setters must return an error code :
    # return 0 if the attribute has been adequately set,
    # return -1 if the attribute was wrong.
    @torch.jit.export
    def set_sr(self, x: int) -> int:
        self.sr = (x, )
        return 0

if __name__ == '__main__':
    # Create your target class
    model = AudioFeatures()
    # Export it to a torchscript model
    model.export_to_ts('features.ts')