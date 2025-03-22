#!/bin/bash
# concat_src_files.sh
# Script to traverse src folder and concatenate all user-generated files with filename delimiters

# Set the output file
OUTPUT_FILE="all_source_files.txt"

# Clear the output file if it exists
> "$OUTPUT_FILE"

# Function to process each file
process_file() {
  local file="$1"
  
  # Skip binary files and non-user files like .git, .DS_Store, etc.
  if [[ -b "$file" || "$file" == */.git/* || "$file" == */.DS_Store || "$file" == */node_modules/* ]]; then
    return
  fi
  
  # Skip very large files (>1MB)
  if [[ $(stat -f%z "$file") -gt 1000000 ]]; then
    echo "Skipping large file: $file" >&2
    return
  fi
  
  # Skip non-text files using file command
  if ! file "$file" | grep -q "text"; then
    echo "Skipping non-text file: $file" >&2
    return
  fi
  
  echo "Processing: $file" >&2
  
  # Add delimiter with filename
  echo -e "\n\n==================== FILE: $file ====================\n" >> "$OUTPUT_FILE"
  
  # Concatenate file content
  cat "$file" >> "$OUTPUT_FILE"
}

# Main function to traverse directory
traverse_directory() {
  local dir="$1"
  
  # Find all files in the directory and subdirectories
  find "$dir" -type f | sort | while read -r file; do
    process_file "$file"
  done
}

echo "Starting file concatenation..."
traverse_directory "src"
echo "File concatenation complete. Output saved to $OUTPUT_FILE" 