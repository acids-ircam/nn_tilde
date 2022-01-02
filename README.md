## Build Instructions

### Windows

- Download Libtorch 1.10 (CPU) and dependencies [here](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-1.10.1%2Bcpu.zip) and unzip to a known directory.
- Run the following commands:

```
git clone https://github.com/caillonantoine/nn_tilde --recurse-submodules
cd nn_tilde
mkdir build
cd build
cmake . -DTorch_DIR="<unzipped libtorch directory>\share\cmake\Torch"
```

- Copy all the DLLs from `<unzipped libtorch directory>\lib` to the Max 7/8 installation directory, next to `Max.exe`
