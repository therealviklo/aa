#pragma once
#include <memory>
#include "llvm.h"
#include "context.h"

struct Scopes;

class Statement
{
public:
	Statement() = default;
	virtual ~Statement() = default;
	
	Statement(const Statement&) = delete;
	Statement& operator=(const Statement&) = delete;

	virtual void writeStatement(Context& c, Scopes& s) const = 0;
};