#! /bin/sh
#
# build nn~ external for puredata on macOS using libtorch from homebrew:
# https://github.com/acids-ircam/nn_tilde
#
# 1. clone nn_tilde ( make sure you clone submodules and nested submodules)
#    $ git submodule update --init --recursive
# 2. install libtorch via homebrew: brew install libtorch
# 3. place this in the nn_tilde directory and make executable:
#    chmod +x build-nn_tilde-macos-brew.sh
# 4. run: ./build-nn_tilde-macos-brew.sh
#
# notes:
# * this will build for the current arch, so should provide native builds for
#   both x86_64 (Intel) and arm64 (M1) systems
# * binary external is placed in root dir
# * there doesn't appear to be a help file, so check the Max docs and/or video
#   tutorials on youtube, ex. https://www.youtube.com/watch?v=dMZs04TzxUI
# * the built external will most likely *not* be portable to other systems as it
#   relies on finding the libtorch dylibs installed by homebrew
#
# Dan Wilcox danomatika.com 2022

# stop on error
set -e

##### variables

PD_APP=/Applications/Pd.app

##### main

# nuke any previous builds to be safe
rm -rf build
rm -rf src/externals
rm -f nn~.pd_darwin

# setup build
mkdir -p build
cd build
export Torch_DIR=/opt/homebrew/
cmake ../src/ -DPUREDATA_INCLUDE_DIR="${PD_APP}/Contents/Resources/src" \
              -DCMAKE_BUILD_TYPE=Release

# build
make
cp frontend/puredata/nn_tilde/nn~.pd_darwin ../
cp ../src/frontend/puredata/nn_tilde/nn~-help.pd ../
cd ../

# add stub helpfile for testing
# if [ ! -e "nn~-help.pd" ] ; then
#    echo "#N canvas 538 97 450 300 12;" > "nn~-help.pd"
#    echo "#X obj 155 105 nn~;"         >> "nn~-help.pd"
#    echo ""                            >> "nn~-help.pd"
# fi
