#ifndef EX_PASS_H
#define EX_PASS_H

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <map>

namespace {

  struct OpCounter: llvm::PassInfoMixin<OpCounter> {
    // Main entry point, takes IR unit to run the pass on (&F) and the
    // corresponding pass manager (to be queried if need be)
    //
    llvm::PreservedAnalyses
    run(llvm::Function &F, llvm::FunctionAnalysisManager &);

    // Without isRequired returning true, this pass will be skipped for
    // functions decorated with the optnone LLVM attribute. Note that clang -O0
    // decorates all functions with optnone.
    //
    static bool isRequired() { return true; }

    private:
      std::map<std::string, int> opCounter;
  };

}

// The pass registration routine. To run it, use the tag "op-counter", like so:
// opt -load-pass-plugin libOpCounter.dylib -passes=op-counter \
//   -disable-output f.bc
//
llvm::PassPluginLibraryInfo getOpCounterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "OpCounter", LLVM_VERSION_STRING,
    [](llvm::PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
          [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
            llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
            if (Name == "op-counter") {
              FPM.addPass(OpCounter());
              return true;
            }
            return false;
          });
    }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize OpCounter when added to the pass pipeline on the
// command line, i.e. via '-passes=op-counter'
//
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getOpCounterPluginInfo();
}

#endif
