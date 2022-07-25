#pragma once
#include "expression.h"
#include "reftype.h"

class Subscript : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::shared_ptr<Expression> sub;
public:
	Subscript(std::shared_ptr<Expression> expr, std::shared_ptr<Expression> sub) :
		expr(std::move(expr)),
		sub(std::move(sub)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};