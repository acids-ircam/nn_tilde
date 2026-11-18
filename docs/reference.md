# Functionalities

- Max/MSP: [`nn~`](#nn-max) · [`mc.nn~`](#mcnn) · [`mcs.nn~`](#mcsnn) · [`nn.info`](#nninfo)
- PureData: [`nn~`](#nn-puredata)

In the tables below, `<x>` is a required value and `[x]` an optional one. Model names can be given with or without `.ts`. They are searched first in the package's `models/` folder, then in the Max/Pd search path.

---

## `nn~` (Max)

Runs one method of a model on audio signals.

### Arguments

```
nn~ <model> [method] [buffer size] [inlets] [outlets]
nn~ void [inlets] [outlets] [buffer size]
```

| argument | default | description |
|---|---|---|
| `model` | | model name or path. `void` creates an object without model (see [void mode](description.md#lazy-initialisation-void-mode-max)) |
| `method` | `forward` | method of the model to run |
| `buffer size` | largest ratio of the model | internal buffer, in samples (rounded up to a power of two). `0`: smallest buffer, computed in the audio thread. See [Buffering](description.md#buffering) |
| `inlets` | number of method inputs | force the number of inlets |
| `outlets` | number of method outputs | force the number of outlets |

### Inlets and outlets

One signal inlet per method input, one signal outlet per method output. The first inlet also receives messages.

### Attributes

| attribute | type | default | description |
|---|---|---|---|
| `@enable` | int (0/1) | 1 | enables computation. `0` acts as a bypass: outputs are silent and the model is not computed |
| `@gpu` | int (0/1) | 0 | runs the model on GPU (CUDA, or Metal on Apple Silicon) when available |
| `@track_buffers` | int (0/1) | 0 | sends a [buffer attribute](description.md#attributes) to the model again each time its `buffer~` is modified or replaced |

### Messages

| message | description |
|---|---|
| `load <model>` | loads another model, keeping the current method. Safe while the DSP runs |
| `reload` | reloads the current model file from disk |
| `method <name>` | switches to another method of the model. Safe while the DSP runs |
| `set <attribute> <values...>` | sets a model attribute. Buffer and tensor values are given by name: `set table mybuffer` |
| `get <attribute>` | prints the current value of a model attribute |
| `get_attributes` | prints the attributes of the model |
| `get_methods` | prints the methods of the model |
| `dump` | prints the model path, the number of inputs/outputs, ratios, methods and attributes |
| `print_available_models` | prints the models downloadable from the IRCAM Forum API |
| `download <model> [name]` | downloads a model into the package's `models/` folder, optionally renamed |
| `delete <model>` | deletes a downloaded model |
| `enable <0/1>`, `gpu <0/1>`, `track_buffers <0/1>` | same as the attributes |

---

## `mc.nn~`

Multichannel `nn~` where **each channel of a cable is one example** of a batch. The model is called once for all examples. See [Multichannel objects](description.md#multichannel-objects-max).

### Arguments

Same as [`nn~`](#arguments):

```
mc.nn~ <model> [method] [buffer size] [inlets] [outlets]
mc.nn~ void [inlets] [outlets] [buffer size]
```

### Inlets and outlets

One multichannel inlet per method input, one multichannel outlet per method output. The batch size is the largest number of channels among the inlets.

### Attributes

All the attributes of `nn~`, plus:

| attribute | type | default | description |
|---|---|---|---|
| `@chans` | int | batch size | fixed number of channels for the outlets |

### Messages

Same as [`nn~`](#messages).

---

## `mcs.nn~`

Multichannel `nn~` where **each inlet/outlet is one example**, and the channels of a cable are the model's inputs or outputs (e.g. the 16 latent dimensions of a RAVE encoder). See [Multichannel objects](description.md#multichannel-objects-max).

### Arguments

```
mcs.nn~ <model> [method] [examples] [buffer size]
mcs.nn~ void [examples] [buffer size]
```

| argument | default | description |
|---|---|---|
| `model` | | model name or path, or `void` |
| `method` | `forward` | method to run |
| `examples` | 1 | number of examples, i.e. of inlets and outlets |
| `buffer size` | 4096 | internal buffer, in samples. `0`: computed in the audio thread |

### Inlets and outlets

One multichannel inlet and one multichannel outlet per example. Each inlet expects as many channels as the method has inputs (a warning is printed otherwise). Each outlet has as many channels as the method has outputs.

### Attributes

All the attributes of `nn~`, plus:

| attribute | type | default | description |
|---|---|---|---|
| `@chans` | int | number of method outputs | fixed number of channels for each outlet |

### Messages

Same as [`nn~`](#messages).

---

## `nn.info`

Inspects a model without running it, and browses the models available for download.

### Arguments

```
nn.info [model]
```

### Outlets

| outlet | output |
|---|---|
| 1 | model path (`none` if no model) |
| 2 | methods, one `method <name>` message per method |
| 3 | attributes, one `attribute <name>` message per attribute |
| 4 | processing parameters (answer to `parameters`) |
| 5 | model information as a dictionary |
| 6 | models available for download, as a dictionary |

### Attributes

| attribute | type | description |
|---|---|---|
| `@dict` | symbol | name of a dictionary to write the model information into |

### Messages

| message | description |
|---|---|
| `bang`, `dump` | outputs all the information about the model |
| `set <model>` | inspects another model |
| `path` | outputs the model path |
| `methods` | outputs the methods |
| `attributes` | outputs the attributes |
| `parameters <method or attribute>` | for a method: input/output channels and ratios. For an attribute: the type of each value |
| `dump_dict` | outputs the model information as a dictionary |
| `get_available_models` | outputs the models downloadable from the IRCAM Forum API, with their descriptions, as a dictionary |
| `download <model> [name]` | downloads a model, optionally renamed |
| `delete <model>` | deletes a downloaded model |

---

## `nn~` (PureData)

### Arguments

```
nn~ [-m] [-g] [-d] <model> [method] [buffer size]
```

| argument | default | description |
|---|---|---|
| `-m` | | multichannel mode: one multichannel inlet and outlet carrying all inputs and outputs (Pd 0.54+). Required for `load` and `method` |
| `-g`, `-gpu` | | runs the model on GPU when available |
| `-d` | | starts disabled |
| `model` | | model name or path |
| `method` | `forward` | method to run |
| `buffer size` | 4096 | internal buffer, in samples. `0`: computed in the audio thread |

### Inlets and outlets

Without `-m`: one signal inlet per method input, one signal outlet per method output. With `-m`: one multichannel inlet and one multichannel outlet. The last outlet is an **info outlet** for messages.

### Messages

| message | description |
|---|---|
| `enable <0/1>` | enables / bypasses computation |
| `load <model>` | loads another model (`-m` mode only) |
| `reload` | reloads the current model file |
| `method <name>` | switches method (`-m` mode only) |
| `set <attribute> <values...>` | sets a model attribute. Buffer values are given as Pd array names |
| `bufsize <n>` | changes the buffer size |
| `gpu <0/1>` | runs the model on GPU when available |
| `print_available_models` | prints the models downloadable from the IRCAM Forum API |
| `download <model> [name]` | downloads a model, optionally renamed |
| `remove <model>` | deletes a downloaded model |
| `bang` | outputs the state of the object on the info outlet |

### Info outlet

On `bang`, the info outlet outputs:

| message | content |
|---|---|
| `loaded <0/1>` | whether a model is loaded |
| `enabled <0/1>` | whether computation is enabled |
| `gpu <0/1>` | whether the GPU is used |
| `model <path>` | model path |
| `methods <names...>` | methods of the model |
| `method <name>` | current method |
| `attributes <names...>` | attributes of the model |
| `bufsize <n>` | buffer size |
| `dim <in> <out>` | number of inputs and outputs |
| `ratio <in> <out>` | input and output ratios |
