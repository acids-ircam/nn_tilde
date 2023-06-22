set -e
cd /tmp/
curl -L https://download.pytorch.org/libtorch/cpu/libtorch-macos-2.0.0.zip -o libtorch.zip
unzip libtorch.zip
git clone --recursive https://github.com/acids-ircam/nn_tilde.git
cd nn_tilde
mkdir build
cd build
cmake ../src -DCMAKE_PREFIX_PATH=/tmp/libtorch -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64
make
cd ../
mkdir nn_tilde
mkdir nn_tilde/help
mv src/externals nn_tilde
cp src/frontend/maxmsp/nn_tilde/nn~.maxhelp nn_tilde/help
mv nn_tilde ~/Documents/Max\ 8/Packages/