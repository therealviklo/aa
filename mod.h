#pragma once
#include <memory>
#include "expression.h"
#include "commontype.h"
#include "convert.h"

struct Mod : public Expression
{
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;

	Mod(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) :
		left(std::move(left)),
		right(std::move(right)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};