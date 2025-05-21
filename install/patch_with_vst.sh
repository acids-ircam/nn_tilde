#!/bin/bash

# Default values
pd=0
max=0

function print_help() {
    echo "Usage: $0 [--pd_path[=val]] (default: ~/Documents/Pd) [--max[=val]] (by default, look in all ~/Documents/Max X/ ; if specified, look for externals sub-folder)"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --pd=*)
            pd=1
            pd_path="${1#*=}"
            shift
            ;;
        --pd)
            pd=1
            shift
            ;;
        --max=*)
            max=1
            max_path="${1#*=}"
            shift
            ;;
        --max)
            max=1
            shift
            ;;
        --help|-h)
            print_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            shift
            ;;
    esac
done


function patch_max_external() {
    find "$1" -name "nn_tilde" -type d -mindepth 2 -print0 | while IFS= read -r -d '' ext_dir; do
    if [[ -d "$ext_dir/externals" ]]; then
        echo "found nn_tilde at $ext_dir"; 
        find "$ext_dir/externals" -name "*.mxo" -print0 | while IFS= read -r -d '' ext_path; do
            # echo "found external at $ext_path"; 
            ext_name=$(basename "$ext_path")
            find "$ext_dir/support" -name "*.dylib" -print0 | while IFS= read -r -d '' dylib_path; do
                dylib_name=$(basename "$dylib_path")
                echo "fixing library : $dylib_name"
                new_path="/Library/Application Support/ACIDS/RAVE/$dylib_name"
                if [[ ! -e "$new_path" ]]; then 
                    echo "[WARNING] library not found : $new_path. Patch may not work"
                fi
                install_name_tool -change "@loader_path/../../../../support/$dylib_name" "/Library/Application Support/ACIDS/RAVE/$dylib_name" "${ext_path}/Contents/MacOS/${ext_name%.*}" 2> /dev/null
            done
            codesign --deep --force --sign - "${ext_path}/Contents/MacOS/${ext_name%.*}"
        done
    fi
    done
}

function patch_pd_external() {
    ext_dir=$1
    if [[ ! "$(basename $ext_dir)" == "nn_tilde" ]]; then
        ext_dir="${ext_dir}/externals/nn_tilde"
    fi
    if [[ -e $(realpath $ext_dir) ]]; then
        find "$ext_dir" -name "nn~.pd_*" -print0 | while IFS= read -r -d '' ext_path; do
            ext_name=$(basename "$ext_path")
            find "$ext_dir" -name "*.dylib" -print0 | while IFS= read -r -d '' dylib_path; do
                dylib_name=$(basename "$dylib_path")
                echo "fixing library : $dylib_name"
                new_path="/Library/Application Support/ACIDS/RAVE/$dylib_name"
                if [[ ! -e "$new_path" ]]; then 
                    echo "[WARNING] library not found : $new_path. Patch may not work"
                fi
                echo install_name_tool -change "@rpath/$dylib_name" "/Library/Application Support/ACIDS/RAVE/$dylib_name" "${ext_path}" 2> /dev/null
            done
            codesign --deep --force --sign - "${ext_path}"
        done
    else
        echo "folder $ext_dir not found".  
    fi
}


if [[ "$max" -eq 1 ]]; then
    if [[ -n "$max_path" ]]; then 
        patch_max_external "$max_path" 
    else
        find ~/Documents -maxdepth 1 -name "Max *" -type d -print0 | while IFS= read -r -d '' max_dir; do
            if [[ -d $max_dir ]]; then 
                patch_max_external "$max_dir" 
            else 
                echo "$max_dir does not exists" 
            fi
        done 
    fi
fi

if [[ "$pd" -eq 1 ]]; then
    if [[ -n "$max_path" ]]; then 
        patch_pd_external "$pd_path" 
    else
        patch_pd_external "$HOME/Documents/Pd"
    fi
fi