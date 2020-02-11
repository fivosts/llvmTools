#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace llvm;

namespace {
struct Hello : public FunctionPass {
  static char ID;
  Hello() : FunctionPass(ID) {
    std::cout << "I am constructed!" << std::endl;
  }

  bool runOnFunction(Function &F) override {
    errs() << "Hello LLVM";
    std::cout << "Using stdout" << std::endl;
    errs().write_escaped(F.getName()) << '\n';
    outs() << "Using outs()\n";
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
