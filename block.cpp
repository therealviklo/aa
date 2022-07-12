#include "block.h"

void Block::writeStatement(Context& c, Scopes& s) const
{
	Scopes as(&s);
	for (const auto& i : stmts)
	{
		i->writeStatement(c, as);
	}
	if (!c.builder->GetInsertBlock()->getTerminator())
		as.dscope.destroy(c, s);
}