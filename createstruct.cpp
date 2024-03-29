#include "createstruct.h"

llvm::Value* CreateStruct::get(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
	{
		llvm::Value* const mem = createAlloca(st->getType(*c.c), c);
		construct(mem, c, s);
		c.tdscope.add({t, mem});
		return mem;
	}
	throw std::runtime_error("Inte en struct");
}

void CreateStruct::getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const
{
	construct(mem, c, s);
}

std::shared_ptr<Type> CreateStruct::getType(Context& /*c*/, Scopes& /*s*/) const
{
	return makeRef(makeMut(type));
}

void CreateStruct::construct(llvm::Value* mem, Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
	{
		if (s.fscope.contains(st->name))
		{
			const Function& f = s.fscope[st->name];
			std::vector<llvm::Value*> argVals;
			argVals.push_back(mem);
			if (args.size() != f.args.size())
				throw std::runtime_error("Fel antal argument");
			for (size_t i = 0; i < args.size(); i++)
			{
				argVals.push_back(convert(args[i], f.args[i].type, c, s));
			}
			c.builder->CreateCall(f.getFunction(c), argVals);
		}
		else
		{
			if (args.size() != st->fields.size())
				throw std::runtime_error("Antal argument matchar inte antal fält");
			for (size_t i = 0; i < args.size(); i++)
			{
				Assign(std::make_shared<StructMemByNumber>(mem, t, i), args[i]).getValue(c, s);
			}
		}
	}
	else
	{
		throw std::runtime_error("Inte en struct");
	}
}