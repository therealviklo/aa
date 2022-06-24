#pragma once
#include "llvm.h"
#include "statement.h"
#include "scopes.h"
#include "variable.h"
#include "createalloca.h"

class Expression : public Statement
{
private:
	mutable std::shared_ptr<Type> typeCache;
public:
	virtual llvm::Value* getRefValue(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en referens");
	}
	virtual llvm::Value* getValue(Context& c, Scopes& s) const = 0;

	virtual std::shared_ptr<Type> getType(Context& c, Scopes& s) const = 0;
	std::shared_ptr<Type> getTypeC(Context& c, Scopes& s) const;

	virtual std::shared_ptr<Type> getFuncCallReturnType(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en funktion");
	}
	virtual std::vector<std::shared_ptr<Type>> getFuncCallTypes(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en funktion");
	}
	virtual bool isVarargs(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en funktion");
	}
	virtual llvm::FunctionCallee getCallable(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en funktion");
	}

	void writeStatement(Context& c, Scopes& s) const override;

	virtual llvm::Value* getAddress(Context& c, Scopes& s) const;
};