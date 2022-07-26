#pragma once
#include "expression.h"
#include "type.h"

llvm::Value* callPtrReturnConvFun(llvm::Value* mem, std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);
llvm::Value* convert(std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);
llvm::Value* forceConvert(std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType, Context& c, Scopes& s);

class Convert : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::shared_ptr<Type> newType;
public:
	Convert(std::shared_ptr<Expression> expr, std::shared_ptr<Type> newType) :
		expr(expr),
		newType(newType) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	void getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const override;
	bool canPtrReturn(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;

	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};

inline std::string getConvFunName(const Type& from, const Type& to)
{
	const Type* fromValueType = from.getValueType().get();
	if (!fromValueType) fromValueType = &from;
	return fromValueType->getName() + "$$" + to.getName();
}