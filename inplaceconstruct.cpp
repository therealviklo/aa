#include "inplaceconstruct.h"

llvm::Value* InPlaceConstruct::get(Context& c, Scopes& s) const
{
	llvm::Value* const memval = mem->getAddress(c, s);
	cs.construct(memval, c, s);
	return memval;
}

std::shared_ptr<Type> InPlaceConstruct::getType(Context& c, Scopes& s) const
{
	return makeRef(cs.getTypeC(c, s));
}