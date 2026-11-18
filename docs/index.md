![banner](https://github.com/acids-ircam/nn_tilde/raw/master/assets/banner.png)

# nn~ documentation

`nn~` runs deep learning models inside **Max/MSP** and **PureData**, in real time, as audio objects. A model trained in PyTorch is exported once to a TorchScript file (`.ts`); `nn~` then loads it, streams audio and control signals through it, and lets you change its parameters from your patch.

[![RAVE x nn~](http://img.youtube.com/vi/dMZs04TzxUI/mqdefault.jpg)](https://www.youtube.com/watch?v=dMZs04TzxUI)

## Contents

1. **[Installation](installation.md)**: install the Max package or the Pd external, get models, install the Python tools, build from source.
2. **[Detailed description](description.md)**: how `nn~` works: models, methods, ratios, buffering and latency, multichannel objects, threads, model swapping.
3. **[Functionalities](reference.md)**: reference of every object, with its arguments, attributes and messages (`nn~`, `mc.nn~`, `mcs.nn~`, `nn.info`, and `nn~` for Pd).
4. **[Wrapping models with nn_tilde](wrapping.md)**: guide to the Python `nn_tilde` package, and a cookbook of complete, tested examples.

## Quick start (Max)

1. Download the [latest release](https://github.com/acids-ircam/nn_tilde/releases/latest) and unpack it into `Documents/Max 9/Packages/` (or `Max 8`).
2. Get a model, for instance with the message `download <model>` sent to `nn~` (list models with `print_available_models`).
3. Create `[nn~ mymodel forward]`, connect an audio source and turn the DSP on.

## Funding

This work is led at IRCAM, and has been funded by the following projects

* [ANR MakiMono](https://acids.ircam.fr/course/makimono/)
* [ACTOR](https://www.actorproject.org/)
* [DAFNE+](https://dafneplus.eu/) N° 101061548

<img alt="Co-funded by the European Union" src="https://ec.europa.eu/regional_policy/images/information-sources/logo-download-center/eu_co_funded_en.jpg" width=200px/>
