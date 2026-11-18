# Wrapping models with nn_tilde

The `nn_tilde` Python package turns PyTorch code into models that `nn~` can load. This page is a guide to its API, followed by a cookbook of complete examples. Every example has been exported and run in the `nn~` engine; the scripts are in [`docs/examples`](https://github.com/acids-ircam/nn_tilde/tree/master/docs/examples).

**Guide**

1. [Principle](#principle)
2. [A first model](#a-first-model)
3. [Methods](#methods)
4. [Attributes](#attributes)
5. [Tensor attributes](#tensor-attributes)
6. [Buffer attributes](#buffer-attributes)
7. [Keeping a state between calls](#keeping-a-state-between-calls)
8. [Sample rate](#sample-rate)
9. [Streaming convolutions](#streaming-convolutions)
10. [Exporting and testing](#exporting-and-testing)
11. [TorchScript pitfalls](#torchscript-pitfalls)

**Cookbook**

1. [Gain: the minimal model](#1-gain-the-minimal-model)
2. [Stereo tools: several methods, custom setter](#2-stereo-tools-several-methods-custom-setter)
3. [Analysis: audio to control signals](#3-analysis-audio-to-control-signals)
4. [Sine oscillator: state and sample rate](#4-sine-oscillator-state-and-sample-rate)
5. [Wavetable: reading a buffer~](#5-wavetable-reading-a-buffer)
6. [Waveshaper: a tensor attribute set from an array](#6-waveshaper-a-tensor-attribute-set-from-an-array)
7. [Autoencoder: wrapping a trained network](#7-autoencoder-wrapping-a-trained-network)
8. [Streaming convolutional network](#8-streaming-convolutional-network)

---

# Guide

## Principle

A model for `nn~` is a subclass of `nn_tilde.Module` (itself a `torch.nn.Module`) that:

1. **registers methods**: functions that `nn~` can call, with their numbers of input/output channels and their ratios;
2. optionally **registers attributes**: parameters that can be set from Max/Pd;
3. is **exported** to a TorchScript file with `export_to_ts()`.

Every method receives one tensor and returns one tensor, both shaped `(batch, channels, time)`:

- `batch` is 1 with `nn~`, and the number of examples with `mc.nn~` / `mcs.nn~`;
- `channels` is the number of inputs (outputs) declared for the method;
- `time` is the buffer size divided by the input (output) ratio.

Install the package with `pip install nn_tilde` (see [Installation](installation.md#python-tools-for-exporting-models)).

## A first model

```python
import torch
import nn_tilde


class Gain(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        # attributes first: register_method() runs the method once to test it
        self.register_attribute("gain", 1.)
        self.register_method(
            "forward",
            in_channels=1, in_ratio=1,
            out_channels=1, out_ratio=1,
            input_labels=["(signal) input"],
            output_labels=["(signal) amplified input"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # x : (batch, 1, n_samples)
        return x * self.gain[0]


if __name__ == "__main__":
    Gain().export_to_ts("gain.ts")
```

Run the script to get `gain.ts`, put it in `nn_tilde/models/`, then in Max:

```
[nn~ gain]          one inlet, one outlet
[set gain 0.5(      halves the volume
[get gain(          prints "gain: 0.500000"
```

## Methods

```python
self.register_method(
    method_name,              # name of the method of the class
    in_channels, in_ratio,    # inputs
    out_channels, out_ratio,  # outputs
    input_labels=None,        # inlet names (shown in Max)
    output_labels=None,       # outlet names
    test_method=True,         # run the method once to check shapes
    test_buffer_size=8192,    # length of the test signal
)
```

- The method must be decorated with `@torch.jit.export`, except `forward` (always exported).
- **Ratios** set the rate of the signals: with `in_ratio=1, out_ratio=512`, a buffer of `n` samples gives an input of `n` steps and an output of `n // 512` steps, and `nn~` holds each output value for 512 samples. See [Signals, channels and ratios](description.md#signals-channels-and-ratios).
- Use **powers of two** for ratios: the buffer size of `nn~` is a power of two, and must be a multiple of every ratio.
- With `test_method=True`, the method is called on silence with a batch of 1, then of 4. The output must have 3 dimensions, `out_channels` channels, `test_buffer_size // out_ratio` steps, and be `float32`. Registration fails with an explicit error otherwise.
- Since the test runs the method, **register attributes and buffers before methods** that use them.

## Attributes

```python
self.register_attribute("gain", 1.)                  # one value
self.register_attribute("fractal", (2, 0.5))         # several values
self.register_attribute("mode", "tanh")
```

The type of the default value sets the type of the attribute: `bool`, `int`, `float`, `str`, `torch.Tensor` or `nn_tilde.Buffer`. An attribute can hold several values of different types, given as a tuple, and set from Max with `set fractal 4 0.25`.

**Values are stored as a tuple**: read them with `self.gain[0]`, `self.fractal[1]`.

`self.finish()`, at the end of `__init__`, creates a getter `get_<name>` and a setter `set_<name>` for each attribute you have not defined yourself. Write your own setter to validate values. It must:

- be decorated with `@torch.jit.export`;
- take one typed argument per value. TorchScript does not allow `*args`, so a 4-value attribute needs 4 arguments;
- store the values as a tuple;
- return `0` if the value is accepted, `-1` to refuse it. `nn~` then prints an error and the attribute keeps its value.

```python
@torch.jit.export
def set_mode(self, mode: str) -> int:
    if mode not in ["tanh", "clip"]:
        return -1
    self.mode = (mode,)
    return 0
```

A custom getter must have a typed return value (`-> float`, `-> Tuple[int, float]`...).

The name `sr` is reserved (see [Sample rate](#sample-rate)).

## Tensor attributes

An attribute whose default value is a tensor can be set from a Max `[array]` of any length:

```python
self.register_attribute("coefs", torch.tensor([0., 1.]))
...
coefs = self.coefs[0]      # 1D float tensor
```

In Max, send `set coefs myarray`, where `myarray` is the name of a Max array of floats.

See [the waveshaper](#6-waveshaper-a-tensor-attribute-set-from-an-array).

## Buffer attributes

A `nn_tilde.Buffer` attribute gives the model access to a Max `buffer~` or a Pd array:

```python
self.register_attribute("table", nn_tilde.Buffer(None, 16, 65536))
#                                  initial value ^     ^ min and max length (samples), -1 : no limit
```

Declare the attribute type on the class for TorchScript: `table: Tuple[nn_tilde.Buffer]`.

In a method, `self.table[0]` is the `Buffer`:

| member | description |
|---|---|
| `has_value` | `False` until a buffer has been set |
| `value` | float tensor `(channels, samples)` |
| `sr` | sample rate of the buffer (-1 if unknown) |
| `shape` | shape of `value` |

In Max: `set table mybuffer`, where `mybuffer` is a `buffer~`. With `@track_buffers 1`, the model receives the buffer again each time it is modified. A buffer shorter or longer than the limits is ignored: the model keeps the previous content.

## Keeping a state between calls

A method is called once per buffer. To keep information from one call to the next (a phase, a filter memory, a previous frame), store it in a registered buffer and reassign it in the method:

```python
self.register_buffer("phase", torch.zeros(1, 1, 1))
...
if self.phase.shape[0] != x.shape[0]:      # batch size changed (mc.nn~)
    self.phase = torch.zeros(x.shape[0], 1, 1)
...
self.phase = new_phase
```

The batch size changes with the number of channels of `mc.nn~`, and is 4 during the registration test: always check it. Do not keep state in plain Python attributes: TorchScript only keeps tensors, buffers and typed attributes.

## Sample rate

`nn_tilde.Module(sr=44100)` sets a default sample rate. When `nn~` loads the model, it calls `set_sample_rate()` with the current rate of Max/Pd. Read it in methods with `self.get_sample_rate()`, which returns `-1` if no rate is known.

## Streaming convolutions

`nn~` cuts the audio into buffers. A convolution with zero padding sees silence at the edges of each buffer, which creates a click at every buffer boundary. The [`cached_conv`](https://github.com/acids-ircam/cached_conv) package (installed with `nn_tilde`, and used by RAVE) replaces the padding with the end of the previous buffer:

- call `cc.use_cached_conv(True)` **before** building the network;
- use `cc.Conv1d`, `cc.ConvTranspose1d`... instead of the `torch.nn` versions, with `padding=cc.get_padding(kernel_size, mode="causal")`.

The streamed result is then identical to processing the whole signal at once. Train with `cc.use_cached_conv(False)`, and switch it on only for export. See [the streaming example](#8-streaming-convolutional-network).

## Exporting and testing

```python
model = MyModel()
model.export_to_ts("my_model.ts")     # eval() + torch.jit.script() + save
```

Before going to Max, check the model in Python:

```python
x = torch.randn(1, 1, 4096)
print(model.forward(x).shape)                  # call methods directly
model.set_gain(0.5)                            # setters return 0 or -1

ts = torch.jit.load("my_model.ts")             # what nn~ will load
print(ts.get_methods(), ts.get_attributes())
print(ts.forward_params)                       # in_channels, in_ratio, out_channels, out_ratio
```

Export with the same PyTorch version as the libtorch of `nn~` (printed in the Max console), or an older one.

In Max, `dump` prints what `nn~` sees of the model, and [`nn.info`](reference.md#nninfo) lists methods, attributes and their parameters.

## TorchScript pitfalls

Methods are compiled with TorchScript, a subset of Python:

- **Type every argument** that is not a tensor (`x: float`), and the return value of getters.
- **No Python globals** in methods: `HOP = 512` at module level is not visible. Store constants on `self` in `__init__` (`self.hop = HOP`).
- **No NumPy, librosa...** inside methods: only `torch` operations. Precompute what you need in `__init__` and store it as a buffer.
- **Return `float32` tensors**: convert with `.float()` if needed.
- Attribute values are **tuples**: `self.gain[0]`, never `self.gain`.
- Methods other than `forward` need **`@torch.jit.export`**.
- `print()` works, but it runs at every buffer: avoid it in methods.

---

# Cookbook

## 1. Gain: the minimal model

One method, one float attribute, automatic getter and setter. [gain.py](examples/gain.py)

```python
import torch
import nn_tilde


class Gain(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        # attributes first: register_method() runs the method once to test it
        self.register_attribute("gain", 1.)
        self.register_method(
            "forward",
            in_channels=1, in_ratio=1,
            out_channels=1, out_ratio=1,
            input_labels=["(signal) input"],
            output_labels=["(signal) amplified input"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # x : (batch, 1, n_samples)
        return x * self.gain[0]


if __name__ == "__main__":
    Gain().export_to_ts("gain.ts")
```

```
[nn~ gain]
[set gain 0.5(
```

## 2. Stereo tools: several methods, custom setter

Three methods in one model, and a custom setter that refuses negative widths. The same `.ts` gives different objects depending on the method argument. [stereo.py](examples/stereo.py)

```python
import torch
import nn_tilde


class StereoTools(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.register_attribute("width", 1.)
        self.register_method(
            "midside", 2, 1, 2, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) mid", "(signal) side"],
        )
        self.register_method(
            "widen", 2, 1, 2, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) left", "(signal) right"],
        )
        self.register_method(
            "mono", 2, 1, 1, 1,
            input_labels=["(signal) left", "(signal) right"],
            output_labels=["(signal) mono"],
        )
        self.finish()

    @torch.jit.export
    def midside(self, x: torch.Tensor) -> torch.Tensor:
        left, right = x[:, 0], x[:, 1]
        return torch.stack([(left + right) / 2, (left - right) / 2], dim=1)

    @torch.jit.export
    def widen(self, x: torch.Tensor) -> torch.Tensor:
        ms = self.midside(x)
        mid, side = ms[:, 0], ms[:, 1] * self.width[0]
        return torch.stack([mid + side, mid - side], dim=1)

    @torch.jit.export
    def mono(self, x: torch.Tensor) -> torch.Tensor:
        return x.mean(dim=1, keepdim=True)

    # a custom setter: refuse negative widths
    @torch.jit.export
    def set_width(self, width: float) -> int:
        if width < 0:
            return -1
        self.width = (width,)
        return 0


if __name__ == "__main__":
    StereoTools().export_to_ts("stereo.ts")
```

```
[nn~ stereo midside]    2 inlets (L, R) -> 2 outlets (mid, side)
[nn~ stereo widen]      [set width 1.5(   ;  [set width -1( is refused
[nn~ stereo mono]       2 inlets -> 1 outlet
```

## 3. Analysis: audio to control signals

An output ratio of 512 gives one value per 512 samples, held by `nn~` until the next one: here RMS and zero-crossing rate as control signals. [analysis.py](examples/analysis.py)

```python
import torch
import nn_tilde

HOP = 512


class Analysis(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.hop = HOP  # TorchScript cannot read Python globals : keep it on self
        # 1 audio channel in, 2 control channels out, one value every HOP samples
        self.register_method(
            "describe", 1, 1, 2, HOP,
            input_labels=["(signal) input"],
            output_labels=["(signal) rms", "(signal) zero-crossing rate"],
        )
        self.finish()

    @torch.jit.export
    def describe(self, x: torch.Tensor) -> torch.Tensor:
        # x : (batch, 1, n)  ->  frames : (batch, 1, n // hop, hop)
        b, c, n = x.shape
        frames = x.reshape(b, c, n // self.hop, self.hop)
        rms = frames.pow(2).mean(-1).sqrt()
        signs = torch.sign(frames)
        zcr = (signs[..., 1:] != signs[..., :-1]).float().mean(-1)
        # output : (batch, 2, n // hop)
        return torch.cat([rms, zcr], dim=1)


if __name__ == "__main__":
    Analysis().export_to_ts("analysis.ts")
```

```
[nn~ analysis describe]  -> 2 outlets, to [snapshot~] or [scope~]
```

## 4. Sine oscillator: state and sample rate

The phase is kept between calls in a registered buffer, reset when the batch size changes, and the increment uses the sample rate given by `nn~`. [sine.py](examples/sine.py)

```python
import math
import torch
import nn_tilde


class Sine(nn_tilde.Module):
    def __init__(self):
        super().__init__(sr=44100)  # default rate; nn~ sets the real one at load
        # state kept between two calls : one phase per batch
        self.register_buffer("phase", torch.zeros(1, 1, 1))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) frequency (Hz)"],
            output_labels=["(signal) sine"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, freq: torch.Tensor) -> torch.Tensor:
        # the batch size changes with mc.nn~ : reset the state if needed
        if self.phase.shape[0] != freq.shape[0]:
            self.phase = torch.zeros(freq.shape[0], 1, 1)
        increment = freq / float(self.get_sample_rate())
        phase = self.phase + torch.cumsum(increment, dim=-1)
        self.phase = phase[..., -1:] % 1.0
        return torch.sin(2 * math.pi * phase)


if __name__ == "__main__":
    Sine().export_to_ts("sine.ts")
```

```
[sig~ 440]
|
[nn~ sine]
```

With `[mc.nn~ sine]` and a 4-channel cable of frequencies, the phase buffer holds 4 phases.

## 5. Wavetable: reading a buffer~

A `Buffer` attribute: the waveform is read from a Max `buffer~` (first channel), and the model outputs silence until a buffer is set. [wavetable.py](examples/wavetable.py)

```python
from typing import Tuple
import torch
import nn_tilde


class Wavetable(nn_tilde.Module):
    table: Tuple[nn_tilde.Buffer]

    def __init__(self):
        super().__init__(sr=44100)
        # a Max buffer~ / Pd array, between 16 and 65536 samples long
        self.register_attribute("table", nn_tilde.Buffer(None, 16, 65536))
        self.register_buffer("phase", torch.zeros(1, 1, 1))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) frequency (Hz)"],
            output_labels=["(signal) wavetable output"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, freq: torch.Tensor) -> torch.Tensor:
        if self.phase.shape[0] != freq.shape[0]:
            self.phase = torch.zeros(freq.shape[0], 1, 1)
        increment = freq / float(self.get_sample_rate())
        phase = self.phase + torch.cumsum(increment, dim=-1)
        self.phase = phase[..., -1:] % 1.0

        table = self.table[0]
        if not table.has_value:
            return torch.zeros_like(freq)
        # buffer value : (channels, samples); read the first channel
        wave = table.value[0]
        index = ((phase % 1.0) * wave.shape[0]).long().clamp(0, wave.shape[0] - 1)
        return wave[index]


if __name__ == "__main__":
    Wavetable().export_to_ts("wavetable.ts")
```

```
[buffer~ wave]           one period of a waveform, e.g. 100 samples ([sizeinsamps 100(, then [peek~] or a file)
[nn~ wavetable @track_buffers 1]
[set table wave(         ; with @track_buffers 1, edits of "wave" are followed
```

## 6. Waveshaper: a tensor attribute set from an array

A tensor attribute of any length, set from a Max `[array]`: the polynomial coefficients of a waveshaper. [waveshaper.py](examples/waveshaper.py)

```python
import torch
import nn_tilde


class Waveshaper(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        # polynomial coefficients c0 + c1 x + c2 x^2 + ... , any length.
        # In Max, set it from an [array] : "set coefs myarray"
        self.register_attribute("coefs", torch.tensor([0., 1.]))
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) input"],
            output_labels=["(signal) shaped output"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        coefs = self.coefs[0]
        out = torch.zeros_like(x)
        power = torch.ones_like(x)
        for i in range(coefs.shape[0]):
            out = out + coefs[i] * power
            power = power * x
        return torch.tanh(out)


if __name__ == "__main__":
    Waveshaper().export_to_ts("waveshaper.ts")
```

```
[set coefs mycoefs(      mycoefs : a Max array of floats, e.g. 0. 1. 0.5 0.2
```

## 7. Autoencoder: wrapping a trained network

The usual structure for a trained model: a wrapper `nn_tilde.Module` holds the network and exposes `encode`, `decode` and `forward`, with a latent space at 1/256 of the audio rate, like RAVE. Here the network is a toy one; load your trained weights instead. [autoencoder.py](examples/autoencoder.py)

```python
import torch
import torch.nn as nn
import nn_tilde

RATIO = 256      # one latent frame every 256 samples
LATENT = 8       # latent dimensions


class TinyAutoencoder(nn.Module):
    """Stands for your trained model. Kernel size == stride, so each latent
    frame only depends on its own 256 samples : no context needed between
    two nn~ calls."""

    def __init__(self):
        super().__init__()
        self.encoder = nn.Conv1d(1, LATENT, kernel_size=RATIO, stride=RATIO)
        self.decoder = nn.ConvTranspose1d(LATENT, 1, kernel_size=RATIO, stride=RATIO)


class ScriptedAutoencoder(nn_tilde.Module):
    def __init__(self, model: TinyAutoencoder):
        super().__init__()
        self.model = model
        self.register_attribute("temperature", 1.)
        self.register_method(
            "encode", 1, 1, LATENT, RATIO,
            input_labels=["(signal) audio"],
            output_labels=[f"(signal) latent {i}" for i in range(LATENT)],
        )
        self.register_method(
            "decode", LATENT, RATIO, 1, 1,
            input_labels=[f"(signal) latent {i}" for i in range(LATENT)],
            output_labels=["(signal) audio"],
        )
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) audio"],
            output_labels=["(signal) reconstruction"],
        )
        self.finish()

    @torch.jit.export
    def encode(self, x: torch.Tensor) -> torch.Tensor:
        return self.model.encoder(x)

    @torch.jit.export
    def decode(self, z: torch.Tensor) -> torch.Tensor:
        return torch.tanh(self.model.decoder(z * self.temperature[0]))

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.decode(self.encode(x))


if __name__ == "__main__":
    trained = TinyAutoencoder()   # in practice : load your weights here
    ScriptedAutoencoder(trained.eval()).export_to_ts("autoencoder.ts")
```

```
[nn~ autoencoder encode]   1 inlet -> 8 latent outlets (one value per 256 samples)
[nn~ autoencoder decode]   8 latent inlets -> 1 outlet
[set temperature 2.(
```

Because kernel size equals stride, each latent frame only depends on its own 256 samples. Networks with larger kernels need the context of the previous buffer: see the next example.

## 8. Streaming convolutional network

`cached_conv` layers keep the end of each buffer as padding for the next one, so consecutive buffers join without clicks. The script checks it: the second buffer is identical to offline processing. [streaming_conv.py](examples/streaming_conv.py)

```python
import torch
import torch.nn as nn
import cached_conv as cc
import nn_tilde

# Must be called BEFORE building the network : cc.Conv1d then keeps the end of
# each buffer in a cache and uses it as left padding for the next one, so the
# output is identical to processing the whole signal at once (no clicks).
cc.use_cached_conv(True)


class Filter(nn_tilde.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            cc.Conv1d(1, 16, 31, padding=cc.get_padding(31, mode="causal")),
            nn.Tanh(),
            cc.Conv1d(16, 1, 31, padding=cc.get_padding(31, mode="causal")),
        )
        self.register_method(
            "forward", 1, 1, 1, 1,
            input_labels=["(signal) input"],
            output_labels=["(signal) filtered"],
        )
        self.finish()

    @torch.jit.export
    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.net(x)


if __name__ == "__main__":
    model = Filter().eval()
    # check : two consecutive buffers join without discontinuity.
    # (the very first samples differ from offline processing : before the first
    # call the cache holds the response to silence instead of zeros)
    cc.use_cached_conv(False)
    offline = Filter().eval()
    offline.load_state_dict(model.state_dict(), strict=False)
    x = torch.randn(1, 1, 8192)
    with torch.no_grad():
        model(x[..., :4096])
        second_buffer = model(x[..., 4096:])
        error = (second_buffer - offline(x)[..., 4096:]).abs().max().item()
    print(f"streaming error on second buffer: {error:.1e}")
    model.export_to_ts("streaming_conv.ts")
```

```
streaming error on second buffer: 1.8e-07
```
