#pragma once
#include <vector>
#include <memory>
#include "expression.h"
#include "convert.h"
#include "commontype.h"
#include "functiontype.h"

struct FuncCall : public Expression
{
	std::shared_ptr<Expression> name;
	std::vector<std::shared_ptr<Expression>> args;

	FuncCall(std::shared_ptr<Expression> name, std::vector<std::shared_ptr<Expression>> args) :
		name(std::move(name)),
		args(std::move(args)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};