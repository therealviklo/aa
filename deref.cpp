#include "deref.h"

llvm::Value* Deref::get(Context& c, Scopes& s) const
{
	return expr->getValue(c, s);
}

std::shared_ptr<Type> Deref::getType(Context& c, Scopes& s) const
{
	return makeRef(getValueType(expr->getTypeC(c, s))->getTypePointedTo());
}