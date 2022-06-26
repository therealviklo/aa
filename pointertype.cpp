#include "pointertype.h"

bool PointerType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const PointerType* const pt = dynamic_cast<const PointerType*>(t2.get()))
	{
		return pt->containedType->isSame(containedType);
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