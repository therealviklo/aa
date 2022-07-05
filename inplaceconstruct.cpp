#include "inplaceconstruct.h"

llvm::Value* InPlaceConstruct::getRefValue(Context& c, Scopes& s) const
{
	llvm::Value* const memval = mem->getRefValue(c, s);
	cs.construct(memval, c, s);
	return memval;
}

llvm::Value* InPlaceConstruct::getValue(Context& c, Scopes& s) const
{
	return c.builder->CreateLoad(
		getTypeC(c, s)->getType(*c.c),
		getRefValue(c, s)
	);
}

std::shared_ptr<Type> InPlaceConstruct::getType(Context& c, Scopes& s) const
{
	return cs.getTypeC(c, s);
}

llvm::Value* InPlaceConstruct::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}