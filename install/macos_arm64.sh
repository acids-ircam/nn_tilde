set -e
cd /tmp/
curl -L https://anaconda.org/pytorch/pytorch/2.0.0/download/osx-arm64/pytorch-2.0.0-py3.10_0.tar.bz2 | tar -xv
git clone --recursive https://github.com/acids-ircam/nn_tilde.git
cd nn_tilde
mkdir build
cd build
cmake ../src -DCMAKE_PREFIX_PATH=/tmp/lib/python3.10/site-packages/torch -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
make
cd ../
mkdir nn_tilde
mkdir nn_tilde/help
mv src/externals nn_tilde
cp src/frontend/maxmsp/nn_tilde/nn~.maxhelp nn_tilde/help
mv nn_tilde ~/Documents/Max\ 8/Packages/