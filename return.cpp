#include "return.h"

void Return::writeStatement(Context& c, Scopes& s) const
{
	if (c.retType->isPtrReturn())
	{
		if (val)
			Assign(
				std::make_shared<TypeNamePair>(
					c.retType,
					Name("ret")
				),
				val
			).writeStatement(c, s);
		c.builder->CreateRetVoid();
	}
	else
	{
		if (val)
			c.builder->CreateRet(convert(*val, c.retType, c, s));
		else
			c.builder->CreateRetVoid();
	}
}