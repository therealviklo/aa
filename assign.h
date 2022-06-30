#pragma once
#include <memory>
#include "expression.h"
#include "convert.h"

struct Assign : public Expression
{
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;

	Assign(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) :
		left(std::move(left)),
		right(std::move(right)) {}
	
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};