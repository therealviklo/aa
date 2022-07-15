#pragma once
#include <algorithm>
#include "type.h"

class FunctionType : public Type
{
public:
	std::shared_ptr<Type> retType;
	std::vector<std::shared_ptr<Type>> argTypes;
	bool varargs;
	
	FunctionType(std::shared_ptr<Type> retType, std::vector<std::shared_ptr<Type>> argTypes, bool varargs) :
		retType(retType),
		argTypes(std::move(argTypes)),
		varargs(varargs) {}

	bool isSame(std::shared_ptr<Type> t) const override;
	llvm::FunctionType* getFunctionType(llvm::LLVMContext& c) const;
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	std::string getName() const override;
	bool isFunctionPointer() const override { return true; }
};