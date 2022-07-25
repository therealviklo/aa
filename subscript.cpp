#include "subscript.h"

llvm::Value* Subscript::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getValueType(expr->getTypeC(c, s));
	if (t->isPointer())
		return
			c.builder->CreateGEP(
				t->getTypePointedTo()->getType(*c.c),
				expr->getValue(c, s),
				sub->getValue(c, s)
			);
	else if (t->isArr())
		return
			c.builder->CreateGEP(
				t->getType(*c.c),
				expr->getAddress(c, s),
				{llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), 0), sub->getValue(c, s)}
			);
	throw std::runtime_error("Kan inte indexera typ");
}

std::shared_ptr<Type> Subscript::getType(Context& c, Scopes& s) const
{
	return makeRef(getValueType(expr->getTypeC(c, s))->getTypePointedTo());
}