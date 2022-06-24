#pragma once
#include "type.h"

struct VoidType : public Type
{
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	bool isVoid() const override { return true; }
	unsigned getRank() const override;
};