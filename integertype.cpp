#include "integertype.h"

bool IntegerType::isSame(std::shared_ptr<Type> t) const
{
	if (const IntegerType* const it = dynamic_cast<const IntegerType*>(t.get()))
	{
		return it->bits == bits && it->unsign == unsign;
	}
	return false;
}

llvm::Type* IntegerType::getType(llvm::LLVMContext& c) const
{
	return llvm::IntegerType::get(c, bits);
}

std::string IntegerType::getName() const
{
	return (unsign ? "u" : "i") + std::to_string(bits);
}

unsigned IntegerType::getRank() const
{
	return bits + unsign;
}