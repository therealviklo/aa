#include "gte.h"

llvm::Value* Gte::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = commonType(left->getTypeC(c, s), right->getTypeC(c, s));
	if (t->isPointer())
		t = s.tscope["uptr"];
	llvm::Value* const l = forceConvert(left, t, c, s);
	llvm::Value* const r = forceConvert(right, t, c, s);
	if (t->isInt())
	{
		if (t->isSigned())
			return c.builder->CreateICmpSGE(l, r);
		else
			return c.builder->CreateICmpUGE(l, r);
	}
	if (t->isFloat())
		return c.builder->CreateFCmpOGE(l, r);
	throw std::runtime_error("Kan inte jämföra värden");
}

std::shared_ptr<Type> Gte::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["bool"];
}