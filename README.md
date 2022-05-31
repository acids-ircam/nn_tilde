![banner](assets/banner.png)

## Build Instructions

### macOS

Pre-built binaries are available in the [release section](https://github.com/acids-ircam/nn_tilde/releases) of this repo.

- Download libtorch 1.10 (CPU) [here](https://download.pytorch.org/libtorch/cpu/libtorch-macos-1.10.1.zip) and unzip it to a known directory
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

### Windows

- Download Libtorch 1.10 (CPU) and dependencies [here](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-1.10.1%2Bcpu.zip) and unzip to a known directory.
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

### Raspberry Pi

While nn~ can be compiled and used on Raspberry Pi, you may have to consider using lighter deep learning models. We currently only support 64bit OS.

Install nn~ for PureData using

```bash
curl -s https://raw.githubusercontent.com/acids-ircam/nn_tilde/master/install/raspberrypi.sh | bash
```
