#pragma once
#include <memory>
#include "llvm.h"
#include "type.h"

struct Context
{
	std::unique_ptr<llvm::LLVMContext> c;
	std::unique_ptr<llvm::IRBuilder<>> builder;
	std::unique_ptr<llvm::Module> mod;
	std::shared_ptr<Type> retType;
};
