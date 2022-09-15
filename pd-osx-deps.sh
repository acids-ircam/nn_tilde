#!/bin/bash
#
# This script finds all of the dependecies and includes them into
# current folder so that it becomes a libdir to be installed into
# ~/Library/Pd or /Library/Pd <hans@eds.org>
#
# Paths that are needed to link LIBS
# This doesnt work. have to put paths in manualy after grep
# Also with local dylibs Pd crashes
# /opt/homebrew/opt/libtorch/lib/
# /opt/homebrew/opt/protobuf/lib/
# /usr/lib/

PD_APP_LIB=`pwd`
DYLIB_PATH=`/opt/homebrew/opt/libtorch/lib/`
echo "Download all dependencies to $PD_APP_LIB"
echo " "
for pd_darwin in *.pd_darwin; do
    LIBS=`otool -L $pd_darwin`
    if [ "x$LIBS" != "x" ]; then
        echo "$pd_darwin is using:"
        echo "$LIBS"
        for lib in $LIBS; do
            #echo "$lib"
            #if echo $lib | grep --quiet '^/usr/lib/.*dylib$'; then
            if echo $lib | grep '^$DYLIB-PATH.*dylib$'; then
                echo "    $lib"
                install -vp $lib $PD_APP_LIB
                new_lib=`echo $lib | sed 's|.*/\(.*\.dylib\)|\1|'`
                install_name_tool -id @loader_path/$new_lib $PD_APP_LIB/$new_lib
                install_name_tool -change $lib @loader_path/$new_lib $pd_darwin
            fi
        done
        echo " "
    fi
done
# Get dependencies recursively
while true; do
    INSTALLED=0
    for dylib in *.dylib; do
        LIBS=`otool -L $dylib`
        if [ "x$LIBS" != "x" ]; then
            echo "$dylib is using:"
            for lib in $LIBS; do
                if echo $lib | grep --quiet '^/usr/local/.*dylib$'; then
                    echo "    $lib"
                    new_lib=`echo $lib | sed 's|.*/\(.*\.dylib\)|\1|'`
                    if [ -e  $PD_APP_LIB/$new_lib ]; then
                        echo "$PD_APP_LIB/$new_lib already exists, skipping copy."
                    else
                        install -vp $lib $PD_APP_LIB
                        INSTALLED=1
                    fi
                    install_name_tool -id @loader_path/$new_lib $PD_APP_LIB/$new_lib
                    install_name_tool -change $lib @loader_path/$new_lib $dylib
                fi
            done
            echo " "
        fi
    done
    if [ $INSTALLED -eq 0 ]; then
        break
    fi
done
