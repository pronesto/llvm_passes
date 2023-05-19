#ifndef EX_PASS_H
#define EX_PASS_H

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace {

  class SCEVEx: public llvm::PassInfoMixin<SCEVEx> {
    public:
      // Main entry point, takes IR unit to run the pass on (&F) and the
      // corresponding pass manager (to be queried if need be)
      //
      llvm::PreservedAnalyses
        run(llvm::Function &F, llvm::FunctionAnalysisManager &);

      // Without isRequired returning true, this pass will be skipped for
      // functions decorated with the optnone LLVM attribute. Note that clang
      // -O0 decorates all functions with optnone.
      //
      static bool isRequired() { return true; }

    private:
      // This function prints out the induction variable of each loop:
      //
      void identifyInductionVariables(
        llvm::Loop* L,
        llvm::ScalarEvolution& SE
      ) const;
  };

}

// The pass registration routine. To run it, use the tag "scev-ex", like so:
// opt -load-pass-plugin libSCEVEx.dylib -passes=scev-ex \
//   -disable-output f.bc
//
llvm::PassPluginLibraryInfo getSCEVExPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SCEVEx", LLVM_VERSION_STRING,
    [](llvm::PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
          [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
            llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
            if (Name == "scev-ex") {
              FPM.addPass(SCEVEx());
              return true;
            }
            return false;
          });
    }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize SCEVEx when added to the pass pipeline on the
// command line, i.e. via '-passes=scev-ex'
//
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSCEVExPluginInfo();
}

#endif
