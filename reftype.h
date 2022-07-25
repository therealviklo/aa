#pragma once
#include "type.h"

class RefType : public Type
{
private:
	std::shared_ptr<Type> t;
	
	bool isSame(std::shared_ptr<Type> t) const override;
public:
	RefType(std::shared_ptr<Type> t) :
		t(t) {}

	llvm::Type* getType(llvm::LLVMContext& c) const override { return llvm::PointerType::get(c, 0); }
	
	std::string getName() const override { return t->getName() + "&"; }
	bool isMut() const override { return t->isMut(); }
	bool isRef() const override { return true; }
	std::shared_ptr<Type> getUnderlyingType() const override { return nullptr; }
	std::shared_ptr<Type> getValueType() const override { return ::getValueType(t); }
};

inline std::shared_ptr<Type> makeRef(std::shared_ptr<Type> t)
{
	return t->isRef() ? t : std::make_shared<RefType>(t);
}