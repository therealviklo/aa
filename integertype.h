#pragma once
#include "type.h"

class IntegerType : public Type
{
private:
	unsigned bits;
	bool unsign;
public:
	IntegerType(unsigned bits, bool unsign) :
		bits(bits),
		unsign(unsign) {}
	
	bool isSame(std::shared_ptr<Type> t) const override;
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	bool isInt() const override { return true; }
	bool isSigned() const override { return !unsign; }
	unsigned getRank() const override;
};