#include "xor.h"

llvm::Value* Xor::get(Context& c, Scopes& s) const
{
	llvm::Value* const l = convert(left, s.tscope["bool"], c, s);
	llvm::Value* const r = convert(right, s.tscope["bool"], c, s);
	return c.builder->CreateXor(l, r);
}

std::shared_ptr<Type> Xor::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["bool"];
}