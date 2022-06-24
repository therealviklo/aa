#include "deref.h"

llvm::Value* Deref::getRefValue(Context& c, Scopes& s) const
{
	return expr->getValue(c, s);
}

llvm::Value* Deref::getValue(Context& c, Scopes& s) const
{
	return c.builder->CreateLoad(getTypeC(c, s)->getType(*c.c), expr->getValue(c, s));
}

std::shared_ptr<Type> Deref::getType(Context& c, Scopes& s) const
{
	return expr->getTypeC(c, s)->getTypePointedTo();
}

llvm::Value* Deref::getAddress(Context& c, Scopes& s) const
{
	return expr->getValue(c, s);
}