#pragma once
#include <memory>
#include "scope.h"
#include "destructionscope.h"
#include "variable.h"
#include "function.h"
#include "type.h"

struct Variable;
using VScope = Scope<Variable>;
struct Function;
using FScope = Scope<Function>;
struct Type;
using TScope = Scope<std::shared_ptr<Type>>;

struct Scopes
{
	VScope vscope;
	FScope fscope;
	TScope tscope;
	DestructionScope dscope;

	Scopes(Scopes* lowerScope = nullptr);
};