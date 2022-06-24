#pragma once
#include "llvm.h"
#include "type.h"
#include "context.h"

struct Variable
{
	llvm::Value* var;
	std::shared_ptr<Type> type;
	bool ref = false;

	llvm::Value* getValue(Context& c);
};