#pragma once
#include <string>
#include <vector>
#include <memory>
#include "statement.h"
#include "type.h"
#include "funcarg.h"

struct Function
{
	std::string name;
	std::vector<FuncArg> args;
	bool varargs;
	std::shared_ptr<Type> retType;
	std::unique_ptr<Statement> body;

	llvm::Function* getFunction(Context& c) const;
};