#pragma once
#include "llvm.h"
#include "statement.h"
#include "scopes.h"
#include "variable.h"
#include "createalloca.h"
#include "functiontype.h"

class Expression : public Statement
{
private:
	mutable std::shared_ptr<Type> typeCache;
public:
	virtual llvm::Value* get(Context& c, Scopes& s) const = 0;
	llvm::Value* getValue(Context& c, Scopes& s) const;
	virtual llvm::Value* getAddress(Context& c, Scopes& s) const;
	virtual llvm::Value* getRefVarAddress(Context& /*c*/, Scopes& /*s*/) const
	{
		throw std::runtime_error("Inte en referensvariabel");
	}
	virtual void getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const;
	virtual bool canPtrReturn(Context& /*c*/, Scopes& /*s*/) const { return false; }

	virtual std::shared_ptr<Type> getType(Context& c, Scopes& s) const = 0;
	std::shared_ptr<Type> getTypeC(Context& c, Scopes& s) const;

	virtual bool isUninitialised() const { return false; }
	virtual bool isRefVar() const { return false; }

	virtual llvm::Value* createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const;
	virtual std::vector<std::shared_ptr<Type>> getCallArgs(Context& c, Scopes& s) const;
	virtual bool isVarargs(Context& c, Scopes& s) const;

	void writeStatement(Context& c, Scopes& s) const override;
};