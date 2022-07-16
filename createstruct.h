#pragma once
#include "expression.h"
#include "structtype.h"
#include "assign.h"
#include "structmembynumber.h"

class CreateStruct : public Expression
{
private:
	std::shared_ptr<Type> type;
	std::vector<std::shared_ptr<Expression>> args;
public:
	CreateStruct(std::shared_ptr<Type> type, std::vector<std::shared_ptr<Expression>> args) :
		type(type),
		args(std::move(args)) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	void getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const override;
	bool canPtrReturn(Context& /*c*/, Scopes& /*s*/) const override { return true; }
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
	llvm::Value* getAddress(Context& c, Scopes& s) const override;

	void construct(llvm::Value* mem, Context& c, Scopes& s) const;
};