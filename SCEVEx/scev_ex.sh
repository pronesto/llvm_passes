#!/bin/zsh

# Example:
# ./scev_ex.sh ../../tests/sumFirstChar.c

if [ $# -lt 1 ]
then
    echo "Syntax: ./scev_ex.sh file.c"
    exit 1
else
  # LLVM tools:
  #
  LLVM_INSTALL_DIR=/Users/fernando/Programs/llvm-project/build
  LLVM_OPT=$LLVM_INSTALL_DIR/bin/opt
  LLVM_CLANG=$LLVM_INSTALL_DIR/bin/clang
  MY_LLVM_LIB=/Users/fernando/Programs/llvm_passes/SCEVEx/build/libSCEVEx.dylib

  # Move to folder where the file is:
  #
  DR_NAME="$(dirname $1)"
  cd $DR_NAME

  # File names:
  #
  FL_NAME=$(basename $1)
  BS_NAME=$(basename $FL_NAME .c)
  LL_NAME="$BS_NAME.ll"

  # Print the name of the file:
  #
  echo "Analyzing $FL_NAME"
  echo "----"

  # Generating the bytecode in SSA form:
  #
  $LLVM_CLANG -Xclang -disable-O0-optnone -c -S -emit-llvm $FL_NAME -o $LL_NAME
  $LLVM_OPT -S -passes=mem2reg $LL_NAME -o $LL_NAME

  # Running the pass:
  #
  $LLVM_OPT -load-pass-plugin $MY_LLVM_LIB -passes=scev-ex \
    -disable-output $LL_NAME

  # Go back to the folder where you were before, but remove the ll file first:
  #
  rm -rf $LL_NAME
  cd -
fi
