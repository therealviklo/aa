#include "dotop.h"

llvm::Value* DotOp::getRefValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		llvm::Value* const v =
			c.builder->CreateGEP(
				expr->getTypeC(c, s)->getType(*c.c),
				expr->getAddress(c, s),
				{
					llvm::ConstantInt::get((*s.tscope)["u32"]->getType(*c.c), 0),
					llvm::ConstantInt::get((*s.tscope)["u32"]->getType(*c.c), st->fieldnames.at(fieldname))
				}
			);
		return v;
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::getValue(Context& c, Scopes& s) const
{
	return
		c.builder->CreateLoad(
			getTypeC(c, s)->getType(*c.c),
			getRefValue(c, s)
		);
}

std::shared_ptr<Type> DotOp::getType(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		return st->fields[st->fieldnames.at(fieldname)];
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}