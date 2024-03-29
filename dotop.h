#pragma once
#include "expression.h"
#include "structtype.h"
#include "muttype.h"
#include "reftype.h"

class DotOp : public Expression
{
private:
	std::shared_ptr<Expression> expr;
	std::string fieldname;
public:
	DotOp(std::shared_ptr<Expression> expr, std::string fieldname) :
		expr(std::move(expr)),
		fieldname(std::move(fieldname)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const override;
	std::vector<std::shared_ptr<Type>> getCallArgs(Context& c, Scopes& s) const override;
};