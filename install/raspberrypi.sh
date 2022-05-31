# INSTALL DEPENDENCIES
sudo apt update
sudo apt install -y cmake build-essential git puredata puredata-dev python3 python3-pip
python3 -m pip install torch
LIBTORCH=$(pwd)/.local/lib/python3.9/site-packages/torch/

# INSTALL NN TILDE
git clone https://github.com/acids-ircam/nn_tilde.git
cd nn_tilde
mkdir build
cd build
cmake ../src/ -DCMAKE_PREFIX_PATH=$LIBTORCH -DCMAKE_BUILD_TYPE=Release
make
sudo mkdir -p /usr/local/lib/pd-externals/
sudo cp frontend/puredata/nn_tilde/nn~.pd_linux /usr/local/lib/pd-externals/