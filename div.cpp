#include "div.h"

llvm::Value* Div::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getTypeC(c, s);
	llvm::Value* const l = convert(*left, t, c, s);
	llvm::Value* const r = convert(*right, t, c, s);
	if (t->isInt())
	{
		if (t->isSigned())
			return c.builder->CreateSDiv(l, r);
		else
			return c.builder->CreateUDiv(l, r);
	}
	if (t->isFloat())
		return c.builder->CreateFDiv(l, r);
	throw std::runtime_error("Kan inte dividera värden");
}

std::shared_ptr<Type> Div::getType(Context& c, Scopes& s) const
{
	return commonType(left->getTypeC(c, s), right->getTypeC(c, s));
}