#pragma once
#include "expression.h"

class SizeOf : public Expression
{
private:
	std::shared_ptr<Type> t;
public:
	SizeOf(std::shared_ptr<Type> t) :
		t(t) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};