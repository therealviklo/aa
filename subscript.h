#pragma once
#include "expression.h"

class Subscript : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::shared_ptr<Expression> sub;
public:
	Subscript(std::shared_ptr<Expression> expr, std::shared_ptr<Expression> sub) :
		expr(std::move(expr)),
		sub(std::move(sub)) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};