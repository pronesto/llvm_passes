#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"

#include "SCEVEx.hpp"

/**
 * There are multiple ways to iterate over the loops in LLVM. Examples:
 * A pre-order iteration:
 *
 * for (llvm::Loop *L: M.getResult<llvm::LoopAnalysis>(F).getLoopsInPreorder()){
 *   identifyInductionVariables(L, SE);
 * }
 *
 * Or a standard C++17 iterator, e.g.:
 *
 * for (auto L: M.getResult<llvm::LoopAnalysis>(F)) {
 *   identifyInductionVariables(L, SE);
 * }
 *
 * However, these methods go over the same loop multiple times, in case we have
 * nested loops. The thing is that loops, in LLVM, are organized into trees, and
 * by iterating over a loop, we iterate over its children loops. And so, I am
 * using old C++11 iterators.
 */

llvm::PreservedAnalyses
SCEVEx::run(llvm::Function &F, llvm::FunctionAnalysisManager &M) {
  llvm::ScalarEvolution &SE = M.getResult<llvm::ScalarEvolutionAnalysis>(F);
  for (llvm::LoopInfo::iterator LI = M.getResult<llvm::LoopAnalysis>(F).begin(),
      LE = M.getResult<llvm::LoopAnalysis>(F).end();
      LI != LE; ++LI) {
    llvm::Loop *L = *LI;
    identifyInductionVariables(L, SE);
  }
  return llvm::PreservedAnalyses::all();
}

void SCEVEx::identifyInductionVariables(
  llvm::Loop* L,
  llvm::ScalarEvolution& SE
) const {
  for (auto* BB : L->getBlocks()) {
    for (auto& Inst : *BB) {
      if (auto* PHI = dyn_cast<llvm::PHINode>(&Inst)) {
        const llvm::SCEV* SCEV = SE.getSCEV(PHI);
        if (SE.containsAddRecurrence(SCEV)) {
          const llvm::SCEVAddRecExpr* AddRecExpr =
            cast<llvm::SCEVAddRecExpr>(SCEV);
          if (AddRecExpr->isAffine()) {
            // Affine induction variable found
            llvm::Value* IndVar = PHI;
            const llvm::SCEV* IncrementSCEV =
              AddRecExpr->getStepRecurrence(SE);
            const llvm::APInt& IncrementValue =
              cast<llvm::SCEVConstant>(IncrementSCEV)->getValue()->getValue();
            llvm::errs() << "Induction Variable: " << *IndVar << "\n";
            llvm::errs() << "Increment Value: " << IncrementValue << "\n";
          }
        }
      }
    }
  }
}
