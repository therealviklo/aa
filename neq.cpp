#include "neq.h"

llvm::Value* Neq::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = commonType(left->getTypeC(c, s), right->getTypeC(c, s));
	if (t->isPointer())
		t = s.tscope["uptr"];
	llvm::Value* const l = forceConvert(left, t, c, s);
	llvm::Value* const r = forceConvert(right, t, c, s);
	if (t->isInt() || t-> isBool())
		return c.builder->CreateICmpNE(l, r);
	if (t->isFloat())
		return c.builder->CreateFCmpONE(l, r);
	throw std::runtime_error("Kan inte jämföra värden");
}

std::shared_ptr<Type> Neq::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["bool"];
}