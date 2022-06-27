#pragma once
#include "type.h"
#include "scopes.h"

class FutureType : public Type
{
private:
	const TScope& tscope;
	std::string name;

	std::shared_ptr<Type> get() const;
public:
	FutureType(const TScope& tscope, std::string name) :
		tscope(tscope),
		name(std::move(name)) {}
	
	bool isSame(std::shared_ptr<Type> t) const override;
	bool isFuture() const override { return !tscope.contains(name); }
	llvm::Type* getType(llvm::LLVMContext& c) const override;
	std::shared_ptr<Type> getUnderlyingType() const override;
};