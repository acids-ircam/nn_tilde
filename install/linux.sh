# Install dependencies
sudo apt update
sudo apt install -y cmake build-essential git puredata puredata-dev python3 python3-pip curl

# Download and install nn_tilde
git clone https://github.com/acids-ircam/nn_tilde
cd nn_tilde

# Download and install libtorch
mkdir lib
cd lib
curl -LO https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-2.0.1%2Bcpu.zip
unzip libtorch-shared-with-deps-2.0.1%2Bcpu.zip
rm -rf libtorch-shared-with-deps-2.0.1%2Bcpu.zip
cd ../

# Setup Build
mkdir build
cd build
export Torch_DIR=../lib/libtorch/share/cmake/Torch
cmake ../src/ -DCMAKE_BUILD_TYPE=Release

# Build and copy to root dir
make
cp frontend/puredata/nn_tilde/nn~.pd_linux ../../
