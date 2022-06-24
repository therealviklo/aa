#include "not.h"

llvm::Value* Not::getValue(Context& c, Scopes& s) const
{
	llvm::Value* const v = convert(*expr, (*s.tscope)["bool"], c, s);
	return c.builder->CreateNot(v);
}

std::shared_ptr<Type> Not::getType(Context& /*c*/, Scopes& s) const
{
	return (*s.tscope)["bool"];
}