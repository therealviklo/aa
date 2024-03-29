#pragma once
#include "expression.h"
#include "type.h"

class BitCast : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::shared_ptr<Type> newType;
public:
	BitCast(std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType) :
		expr(std::move(expr)),
		newType(newType) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};