#pragma once
#include <string>
#include <vector>
#include <memory>
#include "statement.h"
#include "type.h"
#include "funcarg.h"
#include "functiontype.h"
#include "pointertype.h"
#include "muttype.h"

struct Function
{
	std::string name;
	std::vector<FuncArg> args;
	bool varargs;
	std::shared_ptr<Type> retType;
	std::shared_ptr<Type> methodType;
	bool mut;
	std::unique_ptr<Statement> body;

	llvm::Function* getFunction(Context& c) const;
	std::shared_ptr<FunctionType> getFunctionType() const;
};