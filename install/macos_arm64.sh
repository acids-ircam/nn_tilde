set -e
# curl -L https://anaconda.org/pytorch/pytorch/2.5.1/download/osx-arm64/pytorch-2.5.1-py3.11_0.tar.bz2 | tar -xv
git clone --recursive https://github.com/acids-ircam/nn_tilde.git
cd nn_tilde
curl https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-arm64.sh -o ./miniconda.sh
bash ./miniconda.sh -b -u -p ./env
source ./env/bin/activate
pip install -r requirements.txt
bash 
mkdir build
cd build
cmake ../src -DCMAKE_PREFIX_PATH=../lib/python3.10/site-packages/torch -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
make
cd ../
mkdir nn_tilde
mkdir nn_tilde/help
mv src/externals nn_tilde
cp src/frontend/maxmsp/nn_tilde/nn~.maxhelp nn_tilde/help
mv nn_tilde ~/Documents/Max\ 8/Packages/