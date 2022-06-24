#include "return.h"

void Return::writeStatement(Context& c, Scopes& s) const
{
	if (val)
		c.builder->CreateRet(convert(*val, c.retType, c, s));
	else
		c.builder->CreateRetVoid();
}