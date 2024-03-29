#pragma once
#include <memory>
#include "name.h"
#include "type.h"
#include "expression.h"
#include "createalloca.h"
#include "muttype.h"

struct TypeNamePair : public Expression
{
	std::shared_ptr<Type> type;
	Name name;

	TypeNamePair(std::shared_ptr<Type> type, const Name& name) :
		type(type),
		name(name.name) {}
	
	llvm::Value* get(Context& c, Scopes& s) const override;
	llvm::Value* getRefVarAddress(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	bool isUninitialised() const override { return true; }
	bool isRefVar() const override { return type->isRef(); }
};