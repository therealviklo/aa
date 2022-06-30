#pragma once
#include "expression.h"
#include "pointertype.h"

class AddrOf : public Expression
{
private:
	std::shared_ptr<Expression> expr;
public:
	AddrOf(std::shared_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};