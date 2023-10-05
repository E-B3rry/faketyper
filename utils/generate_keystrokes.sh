#!/bin/bash

input_dir="$1"
extensions="${2:-.c}"
output_file="faketyper.out"
ino_file="../faketyper.ino"

if [ -z "$input_dir" ]; then
  echo "Usage: $0 <input_directory> [extensions]"
  echo "Example: $0 /path/to/dir '.c .h'"
  exit 1
fi

IFS=' ' read -ra ext_array <<< "$extensions"

find_files() {
  local dir="$1"
  for ext in "${ext_array[@]}"; do
    find "$dir" -type f -iname "*$ext"
  done
}

rm -f "$output_file"

file_index=1
while read -r file; do
  content=$(sed -e 's/\\/\\\\/g' -e 's/"/\\"/g' -e 's/\t/\\t/g' -e 's/$/\\n/' "$file" | tr -d '\n')
  echo "const char file${file_index}[] PROGMEM = \"$content\";" >> "$output_file"
  file_index=$((file_index + 1))
done < <(find_files "$input_dir")

echo "const char* const files[] PROGMEM = {" >> "$output_file"
for i in $(seq 1 $((file_index - 1))); do
  echo "  file${i}," >> "$output_file"
done
echo "};" >> "$output_file"

echo "Do you want to update the .ino file? (y/n)"
read -r answer
if [ "$answer" == "y" ]; then
  if grep -q "const char file1\[\] PROGMEM" "$ino_file"; then
    escaped_content=$(printf '%q' "$(cat "$output_file")")
    awk -v new_content="$(echo -n "$escaped_content" | sed "s/^$'//;s/'$//")" "
    /const char file1\[\] PROGMEM/ { print new_content; skip = 1 }
      /};/ && skip { skip = 0; next }
      !skip { print }
    " "$ino_file" > "${ino_file}.tmp"
    mv "${ino_file}.tmp" "$ino_file"
    echo "Updated $ino_file"
  else
    echo "Error: Could not find the files variable part in $ino_file"
  fi
else
  echo "Not updating the .ino file."
fi
