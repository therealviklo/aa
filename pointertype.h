#pragma once
#include "type.h"

class PointerType : public Type
{
public:
	std::shared_ptr<Type> containedType;

	PointerType(std::shared_ptr<Type> containedType) :
		containedType(containedType) {}
	
	bool isSame(std::shared_ptr<Type> t) const override;
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	std::string getName() const override { return containedType->getName() + "*"; }
	bool isPointer() const override { return true; }
	unsigned getRank() const override;
	std::shared_ptr<Type> getTypePointedTo() const override;
};