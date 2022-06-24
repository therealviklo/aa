#include "bitcast.h"

llvm::Value* BitCast::getValue(Context& c, Scopes& s) const
{
	return c.builder->CreateBitOrPointerCast(expr->getValue(c, s), newType->getType(*c.c));
}

std::shared_ptr<Type> BitCast::getType(Context& /*c*/, Scopes& /*s*/) const
{
	return newType;
}