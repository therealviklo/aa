#include "stringlit.h"

llvm::Value* StringLit::get(Context& c, Scopes& /*s*/) const
{
	return c.builder->CreateGlobalStringPtr(str);
}

std::shared_ptr<Type> StringLit::getType(Context& /*c*/, Scopes& s) const
{
	return std::make_shared<PointerType>(s.tscope["u8"]);
}