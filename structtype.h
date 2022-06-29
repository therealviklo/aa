#pragma once
#include <map>
#include <vector>
#include "type.h"

class StructType : public Type
{
public:
	std::vector<std::shared_ptr<Type>> fields;
	std::map<std::string, size_t> fieldnames;
	std::string name;
	bool packed;

	StructType(std::vector<std::shared_ptr<Type>> fields, std::map<std::string, size_t> fieldnames, std::string name, bool packed) :
		fields(std::move(fields)),
		fieldnames(std::move(fieldnames)),
		name(std::move(name)),
		packed(packed) {}

	bool isSame(std::shared_ptr<Type> t) const override;
	bool isStruct() const override { return true; }

	llvm::Type* getType(llvm::LLVMContext& c) const override;
};