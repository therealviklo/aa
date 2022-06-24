#pragma once
#include "type.h"

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
	std::shared_ptr<Type> getTypePointedTo() const override { return type; }
};