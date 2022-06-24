#pragma once
#include "expression.h"
#include "pointertype.h"

class Deref : public Expression
{
private:
	std::unique_ptr<Expression> expr;
public:
	Deref(std::unique_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};