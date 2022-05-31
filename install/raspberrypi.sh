# INSTALL DEPENDENCIES
echo "installing compilation tools..."
sudo apt update > /dev/null 2>&1
sudo apt install -y cmake build-essential git puredata puredata-dev python3 python3-pip > /dev/null 2>&1

echo "installing libtorch..."
python3 -m pip install torch > /dev/null 2>&1
LIBTORCH=$(pwd)/.local/lib/python3.9/site-packages/torch/

# INSTALL NN TILDE
echo "configuring nn~..."
git clone https://github.com/acids-ircam/nn_tilde.git > /dev/null 2>&1
cd nn_tilde
mkdir build
cd build
cmake ../src/ -DCMAKE_PREFIX_PATH=$LIBTORCH -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1

echo "compiling nn~..."
make > /dev/null 2>&1
sudo mkdir -p /usr/local/lib/pd-externals/

echo "installing nn~..."
sudo cp frontend/puredata/nn_tilde/nn~.pd_linux /usr/local/lib/pd-externals/
cd ../../
rm -fr nn_tilde

echo "done !"