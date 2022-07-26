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
		c.tdscope.destroy(c, s);
		s.dscope.destroyAll(c, s);
		c.builder->CreateRetVoid();
	}
	else
	{
		if (val)
		{
			llvm::Value* const retval = convert(val, c.retType, c, s);
			c.tdscope.destroy(c, s);
			s.dscope.destroyAll(c, s);
			c.builder->CreateRet(retval);
		}
		else
		{
			s.dscope.destroyAll(c, s);
			c.builder->CreateRetVoid();
		}
	}
}