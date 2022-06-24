#include "integertype.h"

bool IntegerType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const IntegerType* const it = dynamic_cast<const IntegerType*>(t2.get()))
	{
		return it->bits == bits && it->unsign == unsign;
	}
	return false;
}

llvm::Type* IntegerType::getType(llvm::LLVMContext& c) const
{
	return llvm::IntegerType::get(c, bits);
}

unsigned IntegerType::getRank() const
{
	return bits + unsign;
}