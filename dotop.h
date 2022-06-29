#pragma once
#include "expression.h"
#include "structtype.h"
#include "muttype.h"

class DotOp : public Expression
{
private:
	std::unique_ptr<Expression> expr;
	std::string fieldname;
public:
	DotOp(std::unique_ptr<Expression> expr, std::string fieldname) :
		expr(std::move(expr)),
		fieldname(std::move(fieldname)) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const override;
	std::vector<std::shared_ptr<Type>> getCallArgs(Context& c, Scopes& s) const override;
	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};