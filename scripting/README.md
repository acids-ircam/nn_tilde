# Scripting examples in nn~

These examples demonstrate how to write simple scripts to incorporate any type of deep models from PyTorch into MaxMSP (and potentially running on GPU). The examples show a variety of different use cases that also help to understand the input/output shapes relationships.
- `effects.py` : apply simple effects to the input (identical input and output shapes)
- `features.py` : compute spectral descriptors from the PyTorch audio library (each input audio buffer produces a single float as output)
- `unmix.py` : apply the unmix deep source separation model (input is split into 4 different audio streams containing « drums », « vocals », « bass » and « others »)
