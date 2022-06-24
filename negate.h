#pragma once
#include <memory>
#include "expression.h"
#include "convert.h"

struct Neg : public Expression
{
	std::unique_ptr<Expression> expr;

	Neg(std::unique_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};