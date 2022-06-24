#pragma once
#include <map>
#include <vector>
#include "type.h"

class StructType : public Type
{
public:
	std::vector<std::shared_ptr<Type>> fields;
	std::map<std::string, size_t> fieldnames;

	StructType(std::vector<std::shared_ptr<Type>> fields, std::map<std::string, size_t> fieldnames) :
		fields(std::move(fields)),
		fieldnames(std::move(fieldnames)) {}

	bool isSame(std::shared_ptr<Type> t) const override;

	llvm::Type* getType(llvm::LLVMContext& c) const override;
};