#include "sameas.h"

bool SameAs::isSame(std::shared_ptr<Type> t) const
{
	if (const SameAs* const sa = dynamic_cast<const SameAs*>(t.get()))
	{
		const Type& a = *this;
		const Type& b = *t;
		return this->t->isSameReal(sa->t) && (typeid(a) == typeid(b));
	}
	return false;
}