#include "type.h"

bool Type::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> ut = getUnderlyingType();
	const Type& thistype = ut ? *ut : *this;
	const Type& ttype = *getRealType(t);
	return typeid(thistype) == typeid(ttype);
}

size_t Type::getSize(llvm::Module& mod, llvm::LLVMContext& c) const
{
	return mod.getDataLayout().getTypeAllocSize(getType(c));
}