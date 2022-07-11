#include "expression.h"

void Expression::writeStatement(Context& c, Scopes& s) const
{
	getValue(c, s);
}

void Expression::getValuePtrReturn(llvm::Value* /*mem*/, Context& /*c*/, Scopes& /*s*/) const
{
	throw std::runtime_error("Inte pekarreturnerande uttryck");
}

std::shared_ptr<Type> Expression::getTypeC(Context& c, Scopes& s) const
{
	if (typeCache)
		return typeCache;
	else
		return typeCache = getType(c, s);
}

llvm::Value* Expression::createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		return c.builder->CreateCall(llvm::FunctionCallee(ft->getFunctionType(*c.c), getValue(c, s)), args);
	}
	throw std::runtime_error("Inte en funktion");
}

std::vector<std::shared_ptr<Type>> Expression::getCallArgs(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		return ft->argTypes;
	}
	throw std::runtime_error("Inte en funktion");
}

bool Expression::isVarargs(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		return ft->varargs;
	}
	throw std::runtime_error("Inte en funktion");
}

llvm::Value* Expression::getAddress(Context& c, Scopes& s) const
{
	llvm::Value* const var = createAlloca(getTypeC(c, s)->getType(*c.c), c);
	c.builder->CreateStore(getValue(c, s), var);
	return var;
}