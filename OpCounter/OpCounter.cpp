#include "llvm/Support/raw_ostream.h"
#include "OpCounter.hpp"

llvm::PreservedAnalyses
OpCounter::run(llvm::Function &function, llvm::FunctionAnalysisManager &) {
  llvm::errs() << "Function " << function.getName() << '\n';
  for (const llvm::BasicBlock &bb: function) {
    for (const llvm::Instruction &i: bb) {
      opCounter[i.getOpcodeName()]++;
    }
  }
  for (const auto& [op_name, qty]: opCounter) {
    llvm::errs() << op_name << ": " << qty << "\n";
  }
  opCounter.clear();
  return llvm::PreservedAnalyses::all();
}
