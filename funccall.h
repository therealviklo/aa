#pragma once
#include <vector>
#include <memory>
#include "expression.h"
#include "convert.h"
#include "commontype.h"
#include "functiontype.h"

struct FuncCall : public Expression
{
private:
	llvm::Value* call(llvm::Value* ptrRetMem, Context& c, Scopes& s) const;
public:
	std::shared_ptr<Expression> name;
	std::vector<std::shared_ptr<Expression>> args;

	FuncCall(std::shared_ptr<Expression> name, std::vector<std::shared_ptr<Expression>> args) :
		name(std::move(name)),
		args(std::move(args)) {}

	llvm::Value* getValue(Context& c, Scopes& s) const override;
	void getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const override;
	bool canPtrReturn(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;

	llvm::Value* getAddress(Context& c, Scopes& s) const override;
};