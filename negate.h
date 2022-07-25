#pragma once
#include <memory>
#include "expression.h"
#include "convert.h"

struct Neg : public Expression
{
	std::shared_ptr<Expression> expr;

	Neg(std::shared_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};