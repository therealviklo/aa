#include "commontype.h"

std::shared_ptr<Type> commonType(std::shared_ptr<Type> a, std::shared_ptr<Type> b)
{
	if (a->getRank() < b->getRank())
	{
		return b;
	}
	else
	{
		return a;
	}
}