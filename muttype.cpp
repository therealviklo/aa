#include "muttype.h"

std::string MutType::getName() const
{
	return SameAs::getName() + " mut";
}