#include "subscript.h"

llvm::Value* Subscript::getRefValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = expr->getTypeC(c, s);
	if (t->isPointer())
		return
			c.builder->CreateGEP(
				expr->getTypeC(c, s)->getTypePointedTo()->getType(*c.c),
				expr->getValue(c, s),
				sub->getValue(c, s)
			);
	else if (t->isArr())
		return
			c.builder->CreateGEP(
				expr->getTypeC(c, s)->getType(*c.c),
				expr->getAddress(c, s),
				{llvm::ConstantInt::get((*s.tscope)["usize"]->getType(*c.c), 0), sub->getValue(c, s)}
			);
	throw std::runtime_error("Kan inte indexera typ");
}

llvm::Value* Subscript::getValue(Context& c, Scopes& s) const
{
	return
		c.builder->CreateLoad(
			expr->getTypeC(c, s)->getTypePointedTo()->getType(*c.c),
			getRefValue(c, s)
		);
}

std::shared_ptr<Type> Subscript::getType(Context& c, Scopes& s) const
{
	return expr->getTypeC(c, s)->getTypePointedTo();
}

llvm::Value* Subscript::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}