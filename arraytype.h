#pragma once
#include "type.h"
#include "context.h"
#include "scopes.h"

class ArrayType : public Type
{
private:
	std::shared_ptr<Type> type;
	size_t num;
public:
	ArrayType(std::shared_ptr<Type> type, size_t num) :
		type(type),
		num(num) {}

	bool isSame(std::shared_ptr<Type> t) const override;

	llvm::Type* getType(llvm::LLVMContext& c) const override;
	bool isArr() const override { return true; }
	bool isTriviallyDestructible(Scopes& s) const override;
	std::shared_ptr<Type> getTypePointedTo() const override { return type; }

	void destruct(llvm::Value* mem, Context& c, Scopes& s) const override;
};