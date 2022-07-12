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
	if (isFuture())
	{
		std::shared_ptr<Type> t2 = getRealType(t);
		if (const FutureType* const ft = dynamic_cast<const FutureType*>(t2.get()))
		{
			return ft->name == name;
		}
		return false;
	}
	else
	{
		return get()->isSame(t);
	}
}

llvm::Type* FutureType::getType(llvm::LLVMContext& c) const
{
	return get()->getType(c);
}

std::shared_ptr<Type> FutureType::getUnderlyingType() const
{
	return getRealType(get());
}

void FutureType::destruct(llvm::Value* mem, Context& c, Scopes& s) const
{
	return get()->destruct(mem, c, s);
}