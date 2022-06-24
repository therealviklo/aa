#include "negate.h"

llvm::Value* Neg::getValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getTypeC(c, s);
	if (t->isInt())
		return c.builder->CreateNeg(expr->getValue(c, s));
	if (t->isFloat())
		return c.builder->CreateFNeg(expr->getValue(c, s));
	throw std::runtime_error("Kan inte negera värde");
}

std::shared_ptr<Type> Neg::getType(Context& c, Scopes& s) const
{
	return expr->getTypeC(c, s);
}