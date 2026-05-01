#!/bin/bash

mkdir -p testcase
cd testcase

die() { echo "$1"; exit 1; }

# unblock's output as of 2021 Jan 31 is the test case's expected output.
# That reassures us that the 8x8 case hasn't been broken.

run() {
  cmd="../unblock $1 $2"
  eval "$cmd" || die "Command failed: $cmd"
  [ ! -f "$2" ] && die "No output from: $cmd"
  [ ! -s "$2" ] && die "Empty output from: $cmd"
  # Extract the pixels and compare only those.
  # Ignore the metadata, which changes with the libpng version.
  t1="1.ppm"
  t2="2.ppm"
  convert "../test-ok/$2" "$t1"
  convert            "$2" "$t2"
  cmp -s "$t1" "$t2" || die "Output $2 differs from expected ../test-ok/$2"
  rm -f  "$t1" "$t2"
}

for i in 1 2; do
  run "../test-ok/in$i.bmp" "out$i.bmp"
  run "../test-ok/in$i.png" "out$i.png"
done

# upscale x2 will be a separate test case, for 16x16.
# That lets us check if 16x16 outperforms 8x8, as it should.

exit 0
