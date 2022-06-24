#include "sizeof.h"

llvm::Value* SizeOf::getValue(Context& c, Scopes& s) const
{
	return llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), t->getSize(*c.mod, *c.c));
}

std::shared_ptr<Type> SizeOf::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["usize"];
}