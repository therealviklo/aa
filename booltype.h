#pragma once
#include "type.h"

class BoolType : public Type
{
public:
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	bool isBool() const override { return true; }
	unsigned getRank() const override;
};