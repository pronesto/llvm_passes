# Count LLVM opcodes

This pass prints out a table with the opcodes of LLVM instructions in a program.

## building

```
export LLVM_DIR="/Users/fernando/Programs/llvm-project/build/"
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
make
```

## Running

You run the pass on an LLVM IR file. To produce such a file, do:

```
export LLVM_DIR="/Users/fernando/Programs/llvm-project/build/"
$LLVM_DIR/bin/clang -Xclang -disable-O0-optnone -emit-llvm -S file.c -o file.ll
```

Then you can invoke the pass via the `load-pass-plugin` flag, e.g.:

```
export LLVM_DIR="/Users/fernando/Programs/llvm-project/build/"
$LLVM_DIR/bin/opt -load-pass-plugin libOpCounter.dylib -passes=op-counter \
   -disable-output file.ll
```