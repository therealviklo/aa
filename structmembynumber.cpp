#include "structmembynumber.h"

llvm::Value* StructMemByNumber::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getUnderlyingType(type);
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

std::shared_ptr<Type> StructMemByNumber::getType(Context& /*c*/, Scopes& /*s*/) const
{
	std::shared_ptr<Type> exprt = getUnderlyingType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		std::shared_ptr<Type> t = st->fields[num];
		return makeRef(makeMut(t));
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}