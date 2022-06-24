#pragma once
#include <regex>
#include <string>
#include "llvm.h"
#include "expression.h"
#include "function.h"
#include "pointertype.h"

struct Name : public Expression
{
	std::string name;

	Name(std::string name) :
		name(std::move(name)) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getFuncCallReturnType(Context& c, Scopes& s) const override;
	std::vector<std::shared_ptr<Type>> getFuncCallTypes(Context& c, Scopes& s) const override;
	bool isVarargs(Context& c, Scopes& s) const override;
	llvm::FunctionCallee getCallable(Context& c, Scopes& s) const override;
	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};