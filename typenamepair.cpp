#include "typenamepair.h"

llvm::Value* TypeNamePair::getRefValue(Context& c, Scopes& s) const
{
	if (name.name == "ret")
	{
		if (!s.vscope.contains(name.name))
			throw std::runtime_error("Funktionen returnerar inte via pekare");
		if (!s.vscope[name.name].type->isSame(type))
			throw std::runtime_error("ret deklarerades med fel typ");
		return s.vscope[name.name].var;
	}
	if (!s.vscope.scopeContains(name.name))
	{
		llvm::Value* const var = createAlloca(type->getType(*c.c), c);
		s.vscope.add(name.name, {var, type, true});
		s.dscope.add(s.vscope[name.name]);
		return var;
	}
	return s.vscope[name.name].var;
}

llvm::Value* TypeNamePair::getValue(Context& c, Scopes& s) const
{
	getRefValue(c, s);
	return s.vscope[name.name].getValue(c);
}

std::shared_ptr<Type> TypeNamePair::getType(Context& /*c*/, Scopes& /*s*/) const
{
	if (type->isMut())
		return type;
	else
		return std::make_shared<MutType>(type);
}

llvm::Value* TypeNamePair::getAddress(Context& c, Scopes& s) const
{
	return getRefValue(c, s);
}