#pragma once
#include "expression.h"
#include "pointertype.h"
#include "reftype.h"

class Deref : public Expression
{
private:
	std::shared_ptr<Expression> expr;
public:
	Deref(std::shared_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};