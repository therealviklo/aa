#include "type.h"

bool Type::isSame(std::shared_ptr<Type> t) const
{
	const Type& a = *this;
	const Type& b = *t;
	return typeid(a) == typeid(b);
}

bool Type::isSameReal(std::shared_ptr<Type> t) const
{
	const Type* thisType = getRealType().get();
	if (!thisType) thisType = this;
	return thisType->isSame(::getRealType(t));
}

bool Type::isSameUnderlying(std::shared_ptr<Type> t) const
{
	const Type* thisType = getUnderlyingType().get();
	if (!thisType) thisType = this;
	return thisType->isSame(::getUnderlyingType(t));
}

bool Type::isSameValue(std::shared_ptr<Type> t) const
{
	const Type* thisType = getValueType().get();
	if (!thisType) thisType = this;
	return thisType->isSame(::getValueType(t));
}

size_t Type::getSize(llvm::Module& mod, llvm::LLVMContext& c) const
{
	return mod.getDataLayout().getTypeAllocSize(getType(c));
}