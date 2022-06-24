#include "typenamepair.h"

llvm::Value* TypeNamePair::getRefValue(Context& c, Scopes& s) const
{
	if (!s.vscope.scopeContains(name.name))
	{
		llvm::AllocaInst* const var = createAlloca(type->getType(*c.c), c);
		s.vscope.add(name.name, {var, type, true});
		return var;
	}
	return s.vscope[name.name].getValue(c);
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
	return getValue(c, s);
}