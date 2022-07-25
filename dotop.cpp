#include "dotop.h"

llvm::Value* DotOp::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getValueType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		llvm::Value* const v =
			st->getField(expr->getAddress(c, s), st->fieldnames.at(fieldname), c, s);
		return v;
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

std::shared_ptr<Type> DotOp::getType(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getValueType(expr->getTypeC(c, s));
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
			if (expr->getTypeC(c, s)->isMut())
				return makeRef(makeMut(t));
			else
				return makeRef(t);
		}
	}
	throw std::runtime_error("Vänstra operanden till punktoperatorn måste vara en struct");
}

llvm::Value* DotOp::createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const
{
	std::shared_ptr<Type> exprt = getValueType(expr->getTypeC(c, s));
	if (const StructType* const st = dynamic_cast<const StructType*>(exprt.get()))
	{
		const std::string fname = st->name + "$" + fieldname;
		if (s.fscope.contains(fname))
		{
			args.insert(
				s.fscope[fname].retType->isPtrReturn() ? ++args.begin() : args.begin(),
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
	std::shared_ptr<Type> exprt = getValueType(expr->getTypeC(c, s));
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