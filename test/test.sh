#! /bin/sh

mkdir ./output

# echo '-------------'
# echo 'file -> file'
# ../_xcode/Debug/encoder -q 10 -o ./output/out1.drc -i ./buffers/buffer21.bin

echo '-------------'
echo 'cin -> file'
# ../_xcode/Debug/encoder -q 10 -o ./output/out2.drc < ./buffers/buffer21.bin
../_xcode/Debug/encoder -q 10 -o ./output/out2.drc < ./textfile.txt

