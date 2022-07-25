#include "typenamepair.h"

llvm::Value* TypeNamePair::get(Context& c, Scopes& s) const
{
	if (name.name == "ret")
	{
		if (!s.vscope.contains(name.name))
			throw std::runtime_error("Funktionen returnerar inte via pekare");
		if (!s.vscope[name.name].type->isSameUnderlying(type))
			throw std::runtime_error("ret deklarerades med fel typ");
		if (type->isRef())
			return s.vscope[name.name].getValue(c);
		return s.vscope[name.name].var;
	}
	if (!s.vscope.scopeContains(name.name))
	{
		llvm::Value* const var = createAlloca(type->getType(*c.c), c);
		s.vscope.add(name.name, {var, type, true});
		s.dscope.add(s.vscope[name.name]);
		if (type->isRef())
			return s.vscope[name.name].getValue(c);
		return var;
	}
	if (type->isRef())
		return s.vscope[name.name].getValue(c);
	return s.vscope[name.name].var;
}

llvm::Value* TypeNamePair::getRefVarAddress(Context& c, Scopes& s) const
{
	return s.vscope[name.name].var;
}

std::shared_ptr<Type> TypeNamePair::getType(Context& /*c*/, Scopes& /*s*/) const
{
	return makeRef(makeMut(type));
}