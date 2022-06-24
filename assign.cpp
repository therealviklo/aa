#include "assign.h"

llvm::Value* Assign::getValue(Context& c, Scopes& s) const
{
	if (!left->getTypeC(c, s)->isMut())
		throw std::runtime_error("Oföränderlig typ");
	llvm::Value* const var = left->getRefValue(c, s);
	llvm::Value* const val = convert(*right, left->getTypeC(c, s), c, s);
	c.builder->CreateStore(val, var);
	return var;
}

std::shared_ptr<Type> Assign::getType(Context& c, Scopes& s) const
{
	return left->getTypeC(c, s);
}