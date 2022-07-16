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
	std::string getName() const override { return type->getName() + "[" + std::to_string(num) + "]"; }
	bool isArr() const override { return true; }
	size_t getArrSize() const override { return num; }
	bool isTriviallyDestructible(Scopes& s) const override;
	std::shared_ptr<Type> getTypePointedTo() const override { return type; }

	void destruct(llvm::Value* mem, Context& c, Scopes& s) const override;
};