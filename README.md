![banner](https://github.com/acids-ircam/nn_tilde/raw/master/assets/banner.png)

## Build Instructions (MAX/MSP)

### macOS

Pre-built binaries are available in the [release section](https://github.com/acids-ircam/nn_tilde/releases) of this repo.

- Download the latest libtorch (CPU) [here](https://pytorch.org/get-started/locally/) and unzip it to a known directory
- Run the following commands:

```bash
git clone https://github.com/acids-ircam/nn_tilde --recursive
cd nn_tilde
mkdir build
cd build
cmake ../src/ -DCMAKE_PREFIX_PATH=/path/to/libtorch -DCMAKE_BUILD_TYPE=Release
make
```

- Copy the produced `.mxo` external inside `~/Documents/Max 8/Packages/nn_tilde/externals/`

You can build `nn~` for PureData by adding `-DPUREDATA_INCLUDE_DIR=/Applications/Pd-X.XX-X.app/Contents/Resources/src/` to the cmake call. 

### Windows

- Download Libtorch (CPU) and dependencies [here](https://pytorch.org/get-started/locally/) and unzip to a known directory.
- Install Visual Studio and the C++ tools
- Run the following commands:

```bash
git clone https://github.com/acids-ircam/nn_tilde --recurse-submodules
cd nn_tilde
mkdir build
cd build
cmake . -S ..\src  -DCMAKE_BUILD_TYPE:STRING=Release -G "<generator name of your Visual Studio version>" -A x64  -DTorch_DIR="<unzipped libtorch directory>\share\cmake\Torch"
cmake --build . --config Release
```

Use `cmake --help` to find the generator name for your Visual Studio version

- Copy all the DLLs from `<unzipped libtorch directory>\lib` to the Max 7/8 installation directory, next to `Max.exe`


## Build Instructions (Pd)

### macOS

1. clone nn_tilde ( make sure you clone submodules and nested submodules)
   $ git submodule update --init --recursive
2. install libtorch via homebrew: brew install libtorch
3. place this in the nn_tilde directory and make executable:
   chmod +x build-nn_tilde-macos-brew.sh
4. run: ./build-nn_tilde-macos-brew.sh

notes:
* this will build for the current arch, so should provide native builds for
  both x86_64 (Intel) and arm64 (M1) systems
* binary external is placed in root dir
* there doesn't appear to be a help file, so check the Max docs and/or video
  tutorials on youtube, ex. https://www.youtube.com/watch?v=dMZs04TzxUI
* the built external will most likely *not* be portable to other systems as it
  relies on finding the libtorch dylibs installed by homebrew

### Raspberry Pi

While nn~ can be compiled and used on Raspberry Pi, you may have to consider using lighter deep learning models. We currently only support 64bit OS.

Install nn~ for PureData using

```bash
curl -s https://raw.githubusercontent.com/acids-ircam/nn_tilde/master/install/raspberrypi.sh | bash
```

### Models

Some models can be downloaded from:
https://caillonantoine.github.io/NIME_workshop/
