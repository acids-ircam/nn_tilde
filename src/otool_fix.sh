set -e
TARGET_PATH="$(realpath $1)/.."
TARGET_PATH="$(realpath $TARGET_PATH)"
echo "Processing file: $1 to $TARGET_PATH"
mkdir -p "$TARGET_PATH"
otool -L "$1" | grep -o '/.*\.dylib' | while IFS= read -r file; do
    if [ -e $file ]; then
        echo "copying file $file..."
        cp "$file" "$TARGET_PATH"
        # echo install_name_tool -change \"$file\" \"@loader_path/$(basename "$file")\" $1
        # install_name_tool -change $file @loader_path/$(basename "$file") $1
    fi 
done
echo "Completed processing for: $1"