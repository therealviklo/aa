#include "assign.h"

llvm::Value* Assign::getRefValue(Context& c, Scopes& s) const
{
	if (!left->getTypeC(c, s)->isMut())
		throw std::runtime_error("Oföränderlig typ");
	llvm::Value* const var = left->getRefValue(c, s);
	if (!left->isUninitialised() && !left->getTypeC(c, s)->isTriviallyDestructible(s))
		left->getTypeC(c, s)->destruct(var, c, s);
	copy(right, var, left->getTypeC(c, s), c, s);
	return var;
}

llvm::Value* Assign::getValue(Context& c, Scopes& s) const
{
	getRefValue(c, s);
	return left->getValue(c, s);
}

std::shared_ptr<Type> Assign::getType(Context& c, Scopes& s) const
{
	return left->getTypeC(c, s);
}

llvm::Value* Assign::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}