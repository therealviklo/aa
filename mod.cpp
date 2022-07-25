#include "mod.h"

llvm::Value* Mod::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getTypeC(c, s);
	llvm::Value* const l = convert(*left, t, c, s);
	llvm::Value* const r = convert(*right, t, c, s);
	if (t->isInt())
	{
		if (t->isSigned())
			return c.builder->CreateSRem(l, r);
		else
			return c.builder->CreateURem(l, r);
	}
	if (t->isFloat())
		return c.builder->CreateFRem(l, r);
	throw std::runtime_error("Kan inte multiplicera värden");
}

std::shared_ptr<Type> Mod::getType(Context& c, Scopes& s) const
{
	return commonType(left->getTypeC(c, s), right->getTypeC(c, s));
}