#!/bin/bash

# INSTALL SCRIPT FOR ARCH-LINUX / ARCH-LINUX-ARM64

version=0.0.1
usage="-i {u|a|g} -hv"

display_help() {
    echo
    echo "░ nn~ installer ░"
    echo
    echo "   -i,        install location"
    echo "        u     user specific (default)"
    echo "        a     app specific"
    echo "        g     global"
    echo "   -h,        show this help"
    echo "   -v,        script version"
    echo
    exit 0
}

# possible install locations
user_specific=~/.local/lib/pd/extra
application_specific=/usr/lib/pd/extra
global=/usr/local/lib/pd-externals

INSTALL_LOCATION=$user_specific
LIBTORCH=usr/lib/python3.10/site-packages/torch

while getopts i:hv option
    do
    case "${option}" in
        i )
            case ${OPTARG} in
            a)
                INSTALL_LOCATION=$application_specific;;
            u)
                INSTALL_LOCATION=$user_specific;;
            g)
                INSTALL_LOCATION=$global;;
            *)
                INSTALL_LOCATION=$user_specific;;
            esac
            ;;
        h | -help)
            display_help
            ;;
        v | -version)
            echo "nn~ installer v$version"
            exit 0
            ;;
        *)
            echo $usage
            exit 0
            ;;
    esac
done

echo "checking dependencies..."
dependencies=("pd" "git" "cmake" "ninja" "python-pytorch")
for str in ${dependencies[@]}; do
    pacman -Qi $str
    if [[ $? -eq 1 ]]; then
        echo "dependency missing"
        exit 1
    fi
done

echo "cloning nn~ into /tmp..."
cd /tmp; git clone https://github.com/acids-ircam/nn_tilde.git; cd nn_tilde 

echo "configuring build..."
mkdir build; cd build
cmake ../src/ -DCMAKE_PREFIX_PATH=$LIBTORCH -DCMAKE_BUILD_TYPE=Release -G Ninja
echo "compiling nn~..."
ninja;

echo "installing nn~ in $INSTALL_LOCATION..."

if [[ $INSTALL_LOCATION != $user_specific ]]; then
    echo "sudo is needed... changing mode"
    sudo mkdir -p $INSTALL_LOCATION
    sudo cp frontend/puredata/nn_tilde/nn\~.pd_linux $INSTALL_LOCATION
else
    mkdir -p $INSTALL_LOCATION 
    cp frontend/puredata/nn_tilde/nn\~.pd_linux $INSTALL_LOCATION
fi

echo "have fun!"
