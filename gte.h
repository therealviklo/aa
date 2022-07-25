#pragma once
#include <memory>
#include "expression.h"
#include "commontype.h"
#include "convert.h"

struct Gte : public Expression
{
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;

	Gte(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) :
		left(std::move(left)),
		right(std::move(right)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};