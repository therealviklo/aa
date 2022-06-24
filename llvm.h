#pragma once
#include <llvm/Pass.h>
// #include <llvm/PassManager.h>
#include <llvm/ADT/SmallVector.h>
// #include <llvm/Analysis/Verifier.h>
// #include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Host.h>
// using namespace llvm;