#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <image_path>"
  exit 1
fi

# Get the input image path
input_image="$1"

# Extract the directory and filename without extension
output_dir=$(dirname "$input_image")
output_filename=$(basename "$input_image" | cut -f 1 -d '.')

# Define the output BMP path
output_bmp="${output_dir}/${output_filename}.bmp"

# Convert the image using ImageMagick with the specified settings
magick convert "$input_image" -colors 16 -dither FloydSteinberg -colorspace Gray -rotate -90 "$output_bmp"

echo "Converted $input_image to $output_bmp"