#include "futuretype.h"

std::shared_ptr<Type> FutureType::get() const
{
	std::shared_ptr<Type> t = tscope[name];
	if (dynamic_cast<const FutureType*>(t.get()))
		throw std::runtime_error("Försöker undersöka framtida typ");
	return t;
}

bool FutureType::isSame(std::shared_ptr<Type> t) const
{
	return get()->isSame(t);
}

llvm::Type* FutureType::getType(llvm::LLVMContext& c) const
{
	return get()->getType(c);
}