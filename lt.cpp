#include "lt.h"

llvm::Value* Lt::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = commonType(left->getTypeC(c, s), right->getTypeC(c, s));
	if (t->isPointer())
		t = s.tscope["uptr"];
	llvm::Value* const l = forceConvert(*left, t, c, s);
	llvm::Value* const r = forceConvert(*right, t, c, s);
	if (t->isInt())
	{
		if (t->isSigned())
			return c.builder->CreateICmpSLT(l, r);
		else
			return c.builder->CreateICmpULT(l, r);
	}
	if (t->isFloat())
		return c.builder->CreateFCmpOLT(l, r);
	throw std::runtime_error("Kan inte jämföra värden");
}

std::shared_ptr<Type> Lt::getType(Context& /*c*/, Scopes& s) const
{
	return s.tscope["bool"];
}