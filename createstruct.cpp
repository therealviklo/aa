#include "createstruct.h"

llvm::Value* CreateStruct::getRefValue(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> t = getRealType(type);
	if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
	{
		llvm::Value* const mem = createAlloca(st->getType(*c.c), c);
		if (s.fscope.contains(st->name))
		{
			const Function& f = s.fscope[st->name];
			std::vector<llvm::Value*> argVals;
			argVals.push_back(mem);
			if (args.size() != f.args.size())
				throw std::runtime_error("Fel antal argument");
			for (size_t i = 0; i < args.size(); i++)
			{
				argVals.push_back(convert(*args[i], f.args[i].type, c, s));
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
		return mem;
	}
	throw std::runtime_error("Inte en struct");
}

llvm::Value* CreateStruct::getValue(Context& c, Scopes& s) const
{
	return c.builder->CreateLoad(
		getTypeC(c, s)->getType(*c.c),
		getRefValue(c, s)
	);
}

std::shared_ptr<Type> CreateStruct::getType(Context& /*c*/, Scopes& /*s*/) const
{
	return type;
}

llvm::Value* CreateStruct::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}