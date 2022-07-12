#include "arraytype.h"

bool ArrayType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const ArrayType* const at = dynamic_cast<const ArrayType*>(t2.get()))
	{
		return at->type->isSame(type) && at->num == num;
	}
	return false;
}

bool ArrayType::isTriviallyDestructible(Scopes& s) const
{
	return type->isTriviallyDestructible(s);
}

llvm::Type* ArrayType::getType(llvm::LLVMContext& c) const
{
	return llvm::ArrayType::get(type->getType(c), num);
}

void ArrayType::destruct(llvm::Value* mem, Context& c, Scopes& s) const
{
	for (size_t i = 0; i < num; i++)
	{
		type->destruct(
			c.builder->CreateGEP(
				type->getType(*c.c),
				mem,
				{
					llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), 0),
					llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), i)
				}
			),
			c,
			s
		);
	}
}