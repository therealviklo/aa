#pragma once
#include "expression.h"
#include "type.h"

llvm::Value* convert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);
llvm::Value* forceConvert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);

class Convert : public Expression
{
private:
	std::unique_ptr<Expression> expr;
	std::shared_ptr<Type> newType;
public:
	Convert(std::unique_ptr<Expression> expr, std::shared_ptr<Type> newType) :
		expr(std::move(expr)),
		newType(newType) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};