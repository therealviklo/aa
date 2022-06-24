#include "addrof.h"

llvm::Value* AddrOf::getValue(Context& c, Scopes& s) const
{
	return expr->getAddress(c, s);
}

std::shared_ptr<Type> AddrOf::getType(Context& c, Scopes& s) const
{
	return std::make_shared<PointerType>(expr->getTypeC(c, s));
}