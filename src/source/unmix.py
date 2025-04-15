#
# NN~ - Scripting library
# unmix.py : Advanced scripting example for integrating a deep waveform-to-waveform model.
#
# We provide here a simple example of how to use nn~ in order to transform incoming audio.
# In this example, we do not rely on any ML model, but simply apply effects on input buffers.
#
# ACIDS - IRCAM : Philippe Esling, Axel Chemla--Romeu-Santos, Antoine Caillon
#

# System imports
from typing import List, Tuple
import os
import math
# Pytorch imports
import torch
import torch.nn as nn
import torch
import torchaudio
# NN~ imports
import nn_tilde

class Unmix(nn_tilde.Module):

    def __init__(self, 
                 pretrained):
        super().__init__()
        # REGISTER ATTRIBUTES
        self.register_attribute('sr', 44100)
        self.pretrained = pretrained

        # REGISTER METHODS
        self.register_method(
            'forward',
            in_channels=1,
            in_ratio=1,
            out_channels=4,
            out_ratio=1,
            input_labels=['(signal) signal to monitor'],
            output_labels=['drums', 'bass', 'vocals', 'others'],
        )

    @torch.jit.export
    def forward(self, input: torch.Tensor):
        # Preprocess the input buffer (representation)
        in_r = preprocess(input, int(self.sr[0]), int(self.pretrained.sample_rate))
        # Pass through the deep audio separation
        out = self.pretrained(in_r)
        # Return the separated channels
        return out.mean(dim=2)

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

def preprocess(
    audio: torch.Tensor,
    rate: int,
    model_rate: int,
) -> torch.Tensor:
    """                                                                                                                                                                                                                                              
    From an input tensor, convert it to a tensor of shape                                                                                                                                                                                            
    shape=(nb_samples, nb_channels, nb_timesteps). This includes:                                                                                                                                                                                    
    -  if input is 1D, adding the samples and channels dimensions.                                                                                                                                                                                   
    -  if input is 2D                                                                                                                                                                                                                                
        o and the smallest dimension is 1 or 2, adding the samples one.                                                                                                                                                                              
        o and all dimensions are > 2, assuming the smallest is the samples                                                                                                                                                                           
          one, and adding the channel one                                                                                                                                                                                                            
    - at the end, if the number of channels is greater than the number                                                                                                                                                                               
      of time steps, swap those two.                                                                                                                                                                                                                 
    - resampling to target rate if necessary                                                                                                                                                                                                         
                                                                                                                                                                                                                                                     
    Args:                                                                                                                                                                                                                                            
        audio (Tensor): input waveform                                                                                                                                                                                                               
        rate (float): sample rate for the audio                                                                                                                                                                                                      
        model_rate (float): sample rate for the model                                                                                                                                                                                                
                                                                                                                                                                                                                                                     
    Returns:                                                                                                                                                                                                                                         
        Tensor: [shape=(nb_samples, nb_channels=2, nb_timesteps)]                                                                                                                                                                                    
    """
    shape = torch.as_tensor(audio.shape, device=audio.device)

    if len(shape) == 1:
        # assuming only time dimension is provided.                                                                                                                                                                                                  
        audio = audio[None, None, ...]
    elif len(shape) == 2:
        if shape.min() <= 2:
            # assuming sample dimension is missing                                                                                                                                                                                                   
            audio = audio[None, ...]
        else:
            # assuming channel dimension is missing                                                                                                                                                                                                  
            audio = audio[:, None, ...]
    if audio.shape[1] > audio.shape[2]:
        # swapping channel and time                                                                                                                                                                                                                  
        audio = audio.transpose(1, 2)
    if audio.shape[1] > 2:
        audio = audio[..., :2]

    if audio.shape[1] == 1:
        # if we have mono, we duplicate it to get stereo                                                                                                                                                                                             
        audio = torch.repeat_interleave(audio, 2, dim=1)

    if rate != model_rate:
        # we have to resample to model samplerate if needed                                                                                                                                                                                          
        # this makes sure we resample input only once                                                                                                                                                                                                
        audio = torchaudio.functional.resample(audio,
            orig_freq=rate, new_freq=model_rate, resampling_method="sinc_interpolation"
        ).to(audio.device)
    return audio

if __name__ == '__main__':
    pretrained = torch.jit.load("unmix.pt")  # Pretrained weights
    model = Unmix(pretrained)
    model.export_to_ts('unmix.ts')