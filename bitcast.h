#pragma once
#include "expression.h"
#include "type.h"

class BitCast : public Expression
{
private:
	std::unique_ptr<Expression> expr;
	std::shared_ptr<Type> newType;
public:
	BitCast(std::unique_ptr<Expression> expr, std::shared_ptr<Type> newType) :
		expr(std::move(expr)),
		newType(newType) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};