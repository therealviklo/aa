#include "assign.h"

llvm::Value* Assign::get(Context& c, Scopes& s) const
{
	if (!left->getTypeC(c, s)->isMut())
		throw std::runtime_error("Oföränderlig typ");
	if (left->isRefVar())
	{
		if (!left->getTypeC(c, s)->isRef())
			throw std::runtime_error("Inte en referenstyp");
		llvm::Value* const var = left->getRefVarAddress(c, s);
		copy(right, var, left->getType(c, s), c, s);
		return c.builder->CreateLoad(left->getTypeC(c, s)->getType(*c.c), var);
	}
	llvm::Value* const var = left->getAddress(c, s);
	if (!left->isUninitialised() && !getValueType(left->getTypeC(c, s))->isTriviallyDestructible(s))
		left->getTypeC(c, s)->destruct(var, c, s);
	copy(right, var, getValueType(left->getTypeC(c, s)), c, s);
	return var;
}

std::shared_ptr<Type> Assign::getType(Context& c, Scopes& s) const
{
	return makeRef(left->getTypeC(c, s));
}