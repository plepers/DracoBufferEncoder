#! /bin/sh


rm -rf ./javascript

mkdir javascript

mkdir javascript/_wasm

cd javascript/_wasm

EMSDK=/Users/plepers/work/libs/emsdk-portable/emscripten

export EMSCRIPTEN=$EMSDK/1.37.9

cmake ../../submodules/draco \
  -DCMAKE_TOOLCHAIN_FILE=$EMSDK/1.37.9/cmake/Modules/Platform/Emscripten.cmake \
  -DENABLE_WASM=ON \
  -DENABLE_MESH_COMPRESSION=OFF
  

# -DBUILD_AS_WORKER=ON

make

java -jar ../../closure.jar  --compilation_level SIMPLE --js ./draco_decoder.js --js_output_file ./draco_wasm_wrapper.js

cat ../../worker.js ./draco_wasm_wrapper.js > ../draco_wasm_wrapper.js
cp ./draco_decoder.wasm ../draco_decoder.wasm 


cd ../../

mkdir javascript/_asm

cd javascript/_asm

EMSDK=/Users/plepers/work/libs/emsdk-portable/emscripten

export EMSCRIPTEN=$EMSDK/1.37.9

cmake ../../submodules/draco \
  -DCMAKE_TOOLCHAIN_FILE=$EMSDK/1.37.9/cmake/Modules/Platform/Emscripten.cmake \
  -DENABLE_MESH_COMPRESSION=OFF
  

# -DBUILD_AS_WORKER=ON

make

cat ../../worker.js ./draco_decoder.js > ../draco_decoder.js
