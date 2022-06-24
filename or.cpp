#include "or.h"

llvm::Value* Or::getValue(Context& c, Scopes& s) const
{
	llvm::Value* const l = convert(*left, s.tscope["bool"], c, s);
	llvm::Value* const r = convert(*right, s.tscope["bool"], c, s);
	return c.builder->CreateLogicalOr(l, r);
}

std::shared_ptr<Type> Or::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["bool"];
}