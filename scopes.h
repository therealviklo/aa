#pragma once
#include <memory>
#include "scope.h"
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
	std::unique_ptr<VScope> vscope;
	std::unique_ptr<FScope> fscope;
	std::unique_ptr<TScope> tscope;

	Scopes(Scopes* lowerScope = nullptr);
};