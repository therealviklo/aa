#include "pointertype.h"

bool PointerType::isSame(std::shared_ptr<Type> t) const
{
	if (const PointerType* const pt = dynamic_cast<const PointerType*>(t.get()))
	{
		return pt->containedType->isSameReal(containedType);
	}
	return false;
}

llvm::Type* PointerType::getType(llvm::LLVMContext& c) const
{
	return llvm::PointerType::get(c, 0);
}

unsigned PointerType::getRank() const
{
	return 0;
}

std::shared_ptr<Type> PointerType::getTypePointedTo() const
{
	return containedType;
}