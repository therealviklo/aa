#pragma once
#include <regex>
#include <string>
#include "llvm.h"
#include "expression.h"
#include "function.h"
#include "pointertype.h"
#include "reftype.h"

struct Name : public Expression
{
	std::string name;

	Name(std::string name) :
		name(std::move(name)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const override;
};