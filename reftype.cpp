#include "reftype.h"

bool RefType::isSame(std::shared_ptr<Type> t) const
{
	if (const RefType* const rt = dynamic_cast<const RefType*>(t.get()))
	{
		return rt->t->isSameReal(t);
	}
	return false;
}