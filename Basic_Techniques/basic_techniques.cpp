/**
 * @file basic_techniques.cpp
 *
 * @brief Replace all binary operators (add, sub, mult, ...) to multiplication.
 * 		  go to the 'build' folder and run 'cmake ../' and then run 'make'
 *		  opt -load src/libLLVMAllMultiply.so -example -time-passes -debug-pass=Structure < ../../tutorial-llvm/hello.bc > /dev/null
 *
 * @author Rodrigo Rocha
 * Contact: https://github.com/rcorcs
 *
 */

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/IR/Verifier.h"
//#include "llvm/Transforms/Utils/Local.h"
#include <iostream>

using namespace llvm;

// #include <string>
// #include <vector>
#include <iostream>

namespace {
  struct Basic_examples : public ModulePass {
    static char ID;
    Basic_examples() : ModulePass(ID) {
    	std::cout << "Hello module!" << std::endl;
    }

    bool runOnModule(Module &M) override{
        //check here https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
        outs() << "Module name: " << M.getSourceFileName() << "\nToday is the day I die!\n";

        // Constant *hook;
        StringRef name("new_func");
        // hook = M.getOrInsertFunction(name, Type::getVoidTy(M.getContext()));
        // Function *hook_function = cast<Function>(hook);
		// std::vector<Type*> Doubles(3, Type::getDoubleTy(M.getContext()));
		std::vector<Type*> foivos_args = {Type::getInt32Ty(M.getContext()), Type::getInt32Ty(M.getContext())};
		FunctionType *FT = FunctionType::get(Type::getInt32Ty(M.getContext()), foivos_args, false);
		Function *hook_function = Function::Create(FT, Function::ExternalLinkage, name, &M);

		//Lets create and insert a block to the hook_function
		IRBuilder<> block_builder(M.getContext());
		//The next command creates a block. I guess if I call it more times, I will create multiple blocks
		BasicBlock *first_block = BasicBlock::Create(M.getContext(), "entrypoint", hook_function); //"entrypoint" will be the label inserted in the ir code
		block_builder.SetInsertPoint(first_block); 

		//Now lets place an instruction inside

		ConstantInt* intValue = ConstantInt::get(Type::getInt32Ty(M.getContext()), 30, true);
		Value *ret_value = dyn_cast<Value>(intValue);
		block_builder.CreateRet(ret_value);
		verifyFunction(*hook_function);
		//---------------------------------------------------//


		//-----------Dummy example here---------------//
		// IRBuilder<> test_b(&*((*(M.begin())).begin()), (*((*(M.begin())).begin())).begin()); //this instruction creates an IR builder that points to the first block of the first function
		// ConstantInt* intValue = ConstantInt::get(Type::getInt64Ty(M.getContext()), 30, true);
		// Value *ret_value = dyn_cast<Value>(intValue);
		// test_b.CreateRet(ret_value); // and inserts a dummy ret intruction
		//------------------Dummy example end---------//


		//-------Lets add global variables here----------///
		Constant *var_string = ConstantInt::get(Type::getInt32Ty(M.getContext()), 30, true);
		GlobalVariable *var = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), true, GlobalValue::PrivateLinkage, var_string, "foivos_var");
		var->setAlignment(4);

		ConstantInt* const_int_val = ConstantInt::get(M.getContext(), APInt(32,7));
		GlobalVariable* global_2 = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), true, GlobalValue::PrivateLinkage, const_int_val, "another_foivos_global");
		global_2->setAlignment(4);
		// global_2->setInitializer(const_int_val);

		//and this is how to add a global string //

    	Constant *char1 = ConstantInt::get(Type::getInt8Ty(M.getContext()), 72, true);
    	Constant *char2 = ConstantInt::get(Type::getInt8Ty(M.getContext()), 101, true);
    	Constant *char3 = ConstantInt::get(Type::getInt8Ty(M.getContext()), 108, true);
    	Constant *char4 = ConstantInt::get(Type::getInt8Ty(M.getContext()), 108, true);
    	Constant *char5 = ConstantInt::get(Type::getInt8Ty(M.getContext()), 111, true);
    	Constant *char6 = ConstantInt::get(Type::getInt8Ty(M.getContext()), '\00', true);

		Constant *str = ConstantArray::get(ArrayType::get(Type::getInt8Ty(M.getContext()), 6), {char1, char2, char3, char4, char5, char6});
		GlobalVariable *global_string = new GlobalVariable(M, ArrayType::get(Type::getInt8Ty(M.getContext()), 6), true, GlobalValue::PrivateLinkage, str, "hello_var");
		global_string->setAlignment(1);



		for (SymbolTableList<GlobalVariable>::iterator glob = M.global_begin(); glob != M.global_end(); glob++){
			outs() << "Global variable following:\n----------------\n";
			glob->dump();

			outs() << "\n--------------------\n";
		}

		// GlobalVariable *var = new GlobalVariable(M, Type::getInt64Ty(M.getContext()), true, Function::CommonLinkage, var_string, "foivos_var");

		// M.getGlobalList().push_back(var);

		//------------------------------------------------//





		std::vector<CallInst*> deletedInsts;

		for (SymbolTableList<Function>::iterator F = M.begin(); F != M.end(); F++){
			outs() << "Inside ";
			outs().write_escaped((*F).getName());
			outs() << "\n";
			for (SymbolTableList<BasicBlock>::iterator BB = (*F).begin(); BB != (*F).end(); BB++){ //F is an iterator, not a func
				outs() << "Block \n";
				for (SymbolTableList<Instruction>::iterator I = (*BB).begin(); I != (*BB).end(); I++){
					outs() << "Instruction opcode " << (*I).getOpcode() 
							<< " Opcode name: " << (*I).getOpcodeName((*I).getOpcode()) 
							<< " IsTerminator? " << (*I).isTerminator() 
							<< " isFuncletPad? " << (*I).isFuncletPad() << "\n";	
					if ((*I).getOpcode() == 54){  //54 is the opcode of "call" command

						CallInst *call = dyn_cast<CallInst>(I);
						if(call->getCalledFunction()->getName() == "printf"){ //dont replace the printf calls please!
							continue;
						}
						outs() << "WARNING!" << (*F).getName() << " called: " 
								<< call->getCalledFunction()->getName()
								<< " and the call instruction is called: " << I->getName().str() << "\n";
						outs() << "Number of operands: " << call->getNumOperands() << " and arg operands " << call->getNumArgOperands() << "\n"; //the first operand is the function call

						outs() << "Going to explore the passed parameters of the function call \n";
						for (Use* param = call->arg_begin(); param != call->arg_end(); param++){
							// int a =
							// (*param).dump();
							Value *value_arg = param->get();
							value_arg->printAsOperand(outs());
							value_arg->dump();
							value_arg->getType()->dump(); //Type*
							outs() << "\n";
						}

						outs() << "Going to iterate the formal arguments of the func definition\n";
						// for (Argument* arg = call->getCalledFunction()->arg_begin(); arg != call->getCalledFunction()->arg_end(); arg++){
						for (auto arg = call->getCalledFunction()->arg_begin(); arg != call->getCalledFunction()->arg_end(); arg++){

							(*arg).dump();
						}

						outs() << "\n\n Going to replicate the function call...\n";

						//---------------replicating and substituting instructions here--------//

						// Instruction *repl_call = call->clone(); //If I type only this, you get LLVM error
						// BB->getInstList().insert(I, repl_call); //here the instruction is inserted, so you get two identical cals

						// // const Twine name2("testy_instr"); //cannot assign a name to void values error!
						// // repl_call->setName(name2);  //Maybe because the function is a void ?
						// // outs() << "Testing the name ... " << repl_call->getName().str() << "\n";

						// IRBuilder<> builder(repl_call); //here you create another copy of repl_call. Same this as if you put 'call' here
						// builder.CreateCall(hook_function, None); //and you create yet another call instruction. This time to foivos_func

						// // call->replaceAllUsesWith(repl_call);  // I am not sure what this does. Inserting it does not change anything
						// deletedInsts.push_back(call);
						// deletedInsts.push_back(dyn_cast<CallInst>(repl_call));			


						//-------- Ok lets try another way. Say repl_call was never created:------//

						IRBuilder<> builder(call); //here you create another copy of repl_call. Same this as if you put 'call' here
						// Instruction *f_call = builder.CreateCall(hook_function, None, "test"); //and you create yet another call instruction. This time to foivos_func
						std::vector<Value*> f_params = {ConstantInt::get(M.getContext(), APInt(32,7)), ConstantInt::get(M.getContext(), APInt(32,9))};
						Instruction *f_call = builder.CreateCall(hook_function, f_params, "foivos"); //and you create yet another call instruction. This time to foivos_func
						//you can use create call without the "test" thing. If you use "str", then this will be the register in which the call will return the value
						I->replaceAllUsesWith(f_call);  //they must be the same type here, else you get an error. So foivos_function must be i32, if foo that replaced was i32
						deletedInsts.push_back(call);

						//--------------------It works! :)  ------------------------//

					}
					outs() << "\n";
				}
				outs() << "\n\n";
			}
			outs() << "\n\n\n";
		}

		for(auto it = deletedInsts.begin(); it!=deletedInsts.end(); it++){  //here the original command is deleted
			(*it)->eraseFromParent(); // An instruction cannot have any uses when it is being destroyed
		}

        outs() << "\n\n\nCustom function just created: ";
        outs().write_escaped(hook_function->getName());
        outs() << "\n";

        for (SymbolTableList<Function>::iterator F = M.begin(); F != M.end(); F++){

        	outs() << "I am inside func: ";
        	outs().write_escaped((*F).getName());
        	outs() << "\n";
        	for (SymbolTableList<BasicBlock>::iterator BB = (*F).begin(); BB != (*F).end(); BB++){

	        	runOnBasicBlock(*BB);
        	}
        }
        M.dump();
        return false;
    }

    bool runOnBasicBlock(BasicBlock &BB) {
    	outs() << "I am here\n";
    	return false;
    }

  };
}

char Basic_examples::ID = 0;
static RegisterPass<Basic_examples> X("example", "Basic techniques of LLVM API", false, false);