#pragma once
#include <memory>
#include "expression.h"
#include "convert.h"

struct Not : public Expression
{
	std::shared_ptr<Expression> expr;

	Not(std::shared_ptr<Expression> expr) :
		expr(std::move(expr)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};