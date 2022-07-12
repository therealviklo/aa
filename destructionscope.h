#pragma once
#include <vector>
#include <stdexcept>
#include "type.h"

struct TypedValue
{
	std::shared_ptr<Type> type;
	llvm::Value* val;
};

class DestructionScope
{
private:
	std::vector<TypedValue> vars;
	DestructionScope* lowerScope;
public:
	DestructionScope(DestructionScope* lowerScope) :
		lowerScope(lowerScope) {}

	void add(const TypedValue& val)
	{
		vars.emplace_back(val);
	}

	void destroy(Context& c, Scopes& s)
	{
		for (auto i = vars.rbegin(); i != vars.rend(); ++i)
		{
			i->type->destruct(i->val, c, s);
		}
	}

	void destroyAll(Context& c, Scopes& s)
	{
		destroy(c, s);
		if (lowerScope)
			lowerScope->destroyAll(c, s);
	}
};

class TempDestructionScope
{
private:
	std::vector<TypedValue> vars;
public:
	void add(const TypedValue& val)
	{
		vars.emplace_back(val);
	}

	void destroy(Context& c, Scopes& s)
	{
		for (auto i = vars.rbegin(); i != vars.rend(); ++i)
		{
			i->type->destruct(i->val, c, s);
		}
		vars.clear();
	}
};