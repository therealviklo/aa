#pragma once
#include "type.h"

struct VoidType : public Type
{
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	std::string getName() const override { return "void"; }
	bool isVoid() const override { return true; }
	unsigned getRank() const override;
};