#include "add.h"

llvm::Value* Add::get(Context& c, Scopes& s) const
{
	if (getValueType(left->getTypeC(c, s))->isPointer())
	{
		return
			c.builder->CreateGEP(
				getValueType(left->getTypeC(c, s))->getTypePointedTo()->getType(*c.c),
				left->getValue(c, s),
				right->getValue(c, s)
			);
	}
	if (getValueType(right->getTypeC(c, s))->isPointer())
	{
		return
			c.builder->CreateGEP(
				getValueType(right->getTypeC(c, s))->getTypePointedTo()->getType(*c.c),
				right->getValue(c, s),
				left->getValue(c, s)
			);
	}
	std::shared_ptr<Type> t = getTypeC(c, s);
	llvm::Value* const l = convert(*left, t, c, s);
	llvm::Value* const r = convert(*right, t, c, s);
	if (t->isInt())
		return c.builder->CreateAdd(l, r);
	if (t->isFloat())
		return c.builder->CreateFAdd(l, r);
	throw std::runtime_error("Kan inte addera värden");
}

std::shared_ptr<Type> Add::getType(Context& c, Scopes& s) const
{
	if (getValueType(left->getTypeC(c, s))->isPointer())
		return getValueType(left->getTypeC(c, s));
	if (getValueType(right->getTypeC(c, s))->isPointer())
		return getValueType(right->getTypeC(c, s));
	return commonType(left->getTypeC(c, s), right->getTypeC(c, s));
}