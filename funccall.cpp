#include "funccall.h"

llvm::Value* FuncCall::getValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(name->getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		std::vector<llvm::Value*> argvals;
		std::vector<std::shared_ptr<Type>> argTypes = name->getCallArgs(c, s);
		std::vector<std::shared_ptr<Type>>::const_iterator ati = argTypes.begin();
		llvm::Value* ptrRetMem = nullptr;
		if (ft->retType->isPtrReturn())
			argvals.push_back(ptrRetMem = createAlloca(ft->retType->getType(*c.c), c));
		if (name->isVarargs(c, s))
		{
			for (const auto& i : args)
			{
				if (ati == argTypes.end())
				{
					std::shared_ptr<Type> t = i->getTypeC(c, s);
					if (t->isFloat())
					{	
						t = commonType(t, s.tscope["f64"]);
						argvals.push_back(convert(*i, t, c, s));
					}
					else if (t->isInt() || t->isBool())
					{
						if (t->isSigned())
						{
							t = commonType(t, s.tscope["i32"]);
							argvals.push_back(convert(*i, t, c, s));
						}
						else
						{
							t = commonType(t, s.tscope["u32"]);
							argvals.push_back(convert(*i, t, c, s));
						}
					}
					else
					{
						argvals.push_back(i->getValue(c, s));
					}
				}
				else
				{
					argvals.push_back(convert(*i, *ati++, c, s));
				}
			}
		}
		else
		{
			for (const auto& i : args)
			{
				if (ati == argTypes.end())
					throw std::runtime_error("För många argument");
				argvals.push_back(convert(*i, *ati++, c, s));
			}
		}
		if (ati != argTypes.end())
			throw std::runtime_error("För få argument");
		if (ft->retType->isPtrReturn())
		{
			name->createCall(std::move(argvals), c, s);
			return c.builder->CreateLoad(ft->retType->getType(*c.c), ptrRetMem);
		}
		else
		{
			return name->createCall(std::move(argvals), c, s);
		}
	}
	else
	{
		throw std::runtime_error("Inte en funktion");
	}
}

std::shared_ptr<Type> FuncCall::getType(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(name->getTypeC(c, s));
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t.get()))
	{
		return ft->retType;
	}
	throw std::runtime_error("Inte en funktion");
}