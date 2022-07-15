#pragma once
#include "type.h"

enum struct FloatKind
{
	f16,
	f32,
	f64
};

class FloatType : public Type
{
private:
	FloatKind fkind;
public:
	FloatType(FloatKind fkind) :
		fkind(fkind) {}
	
	bool isSame(std::shared_ptr<Type> t) const override;
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	std::string getName() const override ;
	bool isSigned() const override { return true; }
	bool isFloat() const override { return true; }
	unsigned getRank() const override;
};