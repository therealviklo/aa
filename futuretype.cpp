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
		if (const FutureType* const ft = dynamic_cast<const FutureType*>(t.get()))
		{
			return ft->name == name;
		}
		return false;
	}
	else
	{
		return get()->isSameReal(t);
	}
}

void FutureType::destruct(llvm::Value* mem, Context& c, Scopes& s) const
{
	return get()->destruct(mem, c, s);
}