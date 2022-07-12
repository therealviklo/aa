#include "dotop.h"

llvm::Value* DotOp::getRefValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		llvm::Value* const v =
			st->getField(expr->getAddress(c, s), st->fieldnames.at(fieldname), c, s);
		return v;
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::getValue(Context& c, Scopes& s) const
{
	return
		c.builder->CreateLoad(
			getTypeC(c, s)->getType(*c.c),
			getRefValue(c, s)
		);
}

std::shared_ptr<Type> DotOp::getType(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		const std::string fname = st->name + "$" + fieldname;
		if (s.fscope.contains(fname))
		{
			return s.fscope[fname].getFunctionType();
		}
		else
		{
			std::shared_ptr<Type> t = st->fields[st->fieldnames.at(fieldname)];
			if (expr->getTypeC(c, s)->isMut() && !t->isMut())
				return std::make_shared<MutType>(t);
			else
				return t;
		}
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		const std::string fname = st->name + "$" + fieldname;
		if (s.fscope.contains(fname))
		{
			args.insert(
				args.begin(),
				expr->getAddress(c, s)
			);
			return c.builder->CreateCall(s.fscope[fname].getFunction(c), args);
		}
		return Expression::createCall(std::move(args), c, s);
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

std::vector<std::shared_ptr<Type>> DotOp::getCallArgs(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getRealType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		const std::string fname = st->name + "$" + fieldname;
		if (s.fscope.contains(fname))
		{
			std::vector<std::shared_ptr<Type>> args = s.fscope[fname].getFunctionType()->argTypes;
			if (!args.size())
				throw std::runtime_error("Structen är inte en parameter");
			args.erase(args.begin());
			return args;
		}
		return Expression::getCallArgs(c, s);
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}