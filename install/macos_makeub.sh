
if [[ ! -d "nn_tilde_arm64" ]]; then
    echo "[Error] folder nn_tilde_arm64 not found" 
    exit
fi

if [[ ! -d "nn_tilde_x64" ]]; then
    echo "[Error] folder nn_tilde_arm64 not found" 
    exit
fi

if [[ ! -d "nn_tilde" ]]; then
    cp -r nn_tilde_arm64 nn_tilde
fi 

for i in $(find nn_tilde_x64/externals/*/Contents/MacOS -type f -perm -111) 
do 
    echo "UBing file $i..."
    lipo -create "nn_tilde_arm64/externals/$(basename $i).mxo/Contents/MacOS/$(basename $i)" "nn_tilde_x64/externals/$(basename $i).mxo/Contents/MacOS/$(basename $i)" -output "nn_tilde/externals/$(basename $i).mxo/Contents/MacOS/$(basename $i)"
done

for i in $(find nn_tilde_x64/support/*.dylib)
do 
    arch1=""
    arch2=""
    if [[ ! -f "nn_tilde_x64/support/$(basename $i)" || ! -f "nn_tilde_arm64/support/$(basename $i)" ]]
    then 
        echo "skipping $i"
        continue
    fi
    is_ub=$(file "nn_tilde_x64/support/$(basename $i)" | grep -Eo '2 architectures')
    if [[ -n "$is_ub" ]]; then
        echo "$i / universal binary; skipping"
        continue
    fi
    arch1=$(file -b "nn_tilde_x64/support/$(basename $i)" | grep -Eo 'x86_64|arm64')
    arch2=$(file -b "nn_tilde_arm64/support/$(basename $i)" | grep -Eo 'x86_64|arm64')
    if [[ -z "$arch1" || -z "$arch2" ]]; then
        echo "skipping $i"
        continue
    fi
    if [[ ! "$arch1" == "$arch2" ]]; then
        echo "$i / arch1 : $arch1; arch2 : $arch2"
        lipo -create "nn_tilde_arm64/support/$(basename $i)" "nn_tilde_x64/support/$(basename $i)" -output "nn_tilde/support/$(basename $i)"
    fi
done
