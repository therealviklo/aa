#include "structmembynumber.h"

llvm::Value* StructMemByNumber::getRefValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		llvm::Value* const v =
			c.builder->CreateGEP(
				type->getType(*c.c),
				mem,
				{
					llvm::ConstantInt::get(s.tscope["u32"]->getType(*c.c), 0),
					llvm::ConstantInt::get(s.tscope["u32"]->getType(*c.c), num)
				}
			);
		return v;
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* StructMemByNumber::getValue(Context& c, Scopes& s) const
{
	return
		c.builder->CreateLoad(
			getTypeC(c, s)->getType(*c.c),
			getRefValue(c, s)
		);
}

std::shared_ptr<Type> StructMemByNumber::getType(Context& /*c*/, Scopes& /*s*/) const
{
	std::shared_ptr<Type> exprt = getRealType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		std::shared_ptr<Type> t = st->fields[num];
		if (!t->isMut())
			return std::make_shared<MutType>(t);
		else
			return t;
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}