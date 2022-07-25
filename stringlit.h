#pragma once
#include "expression.h"
#include "pointertype.h"

class StringLit : public Expression
{
private:
	std::string str;
public:
	StringLit(std::string str) :
		str(std::move(str)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};