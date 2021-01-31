#!/bin/bash

# This only shows where the standard images came from.
# It need not be rerun.

# Get these standard images, if we haven't already.
wget -q -r -nc "http://sipi.usc.edu/database/download.php?vol=misc&img=4.1.08" -O in1.tiff # 256x256 jellybeans
wget -q -r -nc "http://sipi.usc.edu/database/download.php?vol=misc&img=4.2.03" -O in2.tiff # 512x512 mandrill

for i in 1 2; do
  # Make the standard image blocky.
  # cjpeg would be more lightweight than imagemagick's convert,
  # but it can't read tiff.
  convert -quality 10 in$i.tiff in$i.jpg
  # Make two inputs for a test case.
  convert in$i.jpg BMP3:in$i.bmp
  convert in$i.jpg in$i.png
done
