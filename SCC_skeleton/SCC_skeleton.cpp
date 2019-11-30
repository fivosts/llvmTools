#include "llvm/Pass.h"
// #include "llvm/IR/Function.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace llvm;

namespace {
struct SCC_pass : public CallGraphSCCPass{
  static char ID;

  SCC_pass() : CallGraphSCCPass(ID) {}

  virtual bool doInitialization(CallGraph &CG){

    return false;
  }

  virtual bool runOnSCC(CallGraphSCC &SCC) override {
  //   errs() << "Hello: ";
  //   std::cout << "I am Foivos" << std::endl;
  //   errs().write_escaped(F.getName()) << '\n';
    return false;
  }
  virtual bool doFinalization(CallGraph &CG){

    return false;
  }

// bool MyCallGraphSCCPass::runOnSCC(CallGraphSCC& SCC) override {
    // for (CallGraphSCC::iterator it = SCC.begin(); it != SCC.end(); it++) {
    //     CallGraphNode* node = *it;
    //     Function* func = node->getFunction();
    // }
    // return false;
// }

}; // end of struct Hello
}  // end of anonymous namespace

char SCC_pass::ID = 0;
static RegisterPass<SCC_pass> X("scc", "SCC pass skeleton",
                             false /* Only looks at CFG */,
                             true /* Analysis Pass */);