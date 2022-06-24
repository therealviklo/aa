#include "expression.h"

void Expression::writeStatement(Context& c, Scopes& s) const
{
	getValue(c, s);
}

std::shared_ptr<Type> Expression::getTypeC(Context& c, Scopes& s) const
{
	if (typeCache)
		return typeCache;
	else
		return typeCache = getType(c, s);
}

llvm::Value* Expression::getAddress(Context& c, Scopes& s) const
{
	llvm::AllocaInst* const var = createAlloca(getTypeC(c, s)->getType(*c.c), c);
	c.builder->CreateStore(getValue(c, s), var);
	return var;
}