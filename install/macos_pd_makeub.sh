TARGET_DIR=$1
if [ -z $TARGET_DIR ]; then
    TARGET_DIR="nn_tilde"
fi

echo "${TARGET_DIR}"

if [[ ! -d "${TARGET_DIR}_arm64" ]]; then
    echo "[Error] folder ${TARGET_DIR}_arm64 not found" 
    exit
fi

if [[ ! -d "${TARGET_DIR}_x64" ]]; then
    echo "[Error] folder ${TARGET_DIR}_arm64 not found" 
    exit
fi

if [[ ! -d "${TARGET_DIR}" ]]; then
    cp -r ${TARGET_DIR}_arm64 ${TARGET_DIR}
fi 

lipo -create "${TARGET_DIR}_arm64/nn~.pd_darwin" "${TARGET_DIR}_x64/nn~.pd_darwin" -output "${TARGET_DIR}/nn~.pd_darwin"

for i in $(find ${TARGET_DIR}_x64/*.dylib)
do 
    arch1=""
    arch2=""
    if [[ ! -f "${TARGET_DIR}_x64$(basename $i)" || ! -f "${TARGET_DIR}_arm64/$(basename $i)" ]]
    then 
        echo "skipping $i"
        continue
    fi
    is_ub=$(file "${TARGET_DIR}_x64/$(basename $i)" | grep -Eo '2 architectures')
    if [[ -n "$is_ub" ]]; then
        echo "$i / universal binary; skipping"
        continue
    fi
    arch1=$(file -b "${TARGET_DIR}_x64/$(basename $i)" | grep -Eo 'x86_64|arm64')
    arch2=$(file -b "${TARGET_DIR}_arm64/$(basename $i)" | grep -Eo 'x86_64|arm64')
    if [[ -z "$arch1" || -z "$arch2" ]]; then
        echo "skipping $i"
        continue
    fi
    if [[ ! "$arch1" == "$arch2" ]]; then
        echo "$i / arch1 : $arch1; arch2 : $arch2"
        lipo -create "${TARGET_DIR}_arm64/$(basename $i)" "${TARGET_DIR}_x64/$(basename $i)" -output "${TARGET_DIR}/$(basename $i)"
    fi
done
