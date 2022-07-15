#pragma once
#include "expression.h"
#include "type.h"

llvm::Value* callPtrReturnConvFun(llvm::Value* mem, const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);
llvm::Value* convert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);
llvm::Value* forceConvert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);

class Convert : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::shared_ptr<Type> newType;
public:
	Convert(std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType) :
		expr(std::move(expr)),
		newType(newType) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	void getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const override;
	bool canPtrReturn() const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;

	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};