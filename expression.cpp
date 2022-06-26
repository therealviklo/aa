#include "expression.h"

void Expression::writeStatement(Context& c, Scopes& s) const
{
	getValue(c, s);
}

std::shared_ptr<Type> Expression::getTypeC(Context& c, Scopes& s) const
{
	if (typeCache)
		return typeCache;
	else
		return typeCache = getType(c, s);
}

llvm::FunctionCallee Expression::getCallable(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		return llvm::FunctionCallee(ft->getFunctionType(*c.c), getValue(c, s));
	}
	throw std::runtime_error("Inte en funktion");
}

llvm::Value* Expression::getAddress(Context& c, Scopes& s) const
{
	llvm::AllocaInst* const var = createAlloca(getTypeC(c, s)->getType(*c.c), c);
	c.builder->CreateStore(getValue(c, s), var);
	return var;
}