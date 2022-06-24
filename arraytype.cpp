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

llvm::Type* ArrayType::getType(llvm::LLVMContext& c) const
{
	return llvm::ArrayType::get(type->getType(c), num);
}