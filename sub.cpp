#include "sub.h"

llvm::Value* Sub::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getTypeC(c, s);
	llvm::Value* const l = convert(left, t, c, s);
	llvm::Value* const r = convert(right, t, c, s);
	if (t->isInt())
		return c.builder->CreateSub(l, r);
	if (t->isFloat())
		return c.builder->CreateFSub(l, r);
	throw std::runtime_error("Kan inte subtrahera värden");
}

std::shared_ptr<Type> Sub::getType(Context& c, Scopes& s) const
{
	return commonType(left->getTypeC(c, s), right->getTypeC(c, s));
}