#pragma once
#include "expression.h"
#include "structtype.h"

class StructMemByNumber : public Expression
{
private:
	llvm::Value* mem;
	std::shared_ptr<Type> type;
	size_t num;
public:
	StructMemByNumber(llvm::Value* mem, std::shared_ptr<Type> type, size_t num) :
		mem(mem),
		type(type),
		num(num) {}

	llvm::Value* getRefValue(Context& c, Scopes& s) const override;
	llvm::Value* getValue(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};