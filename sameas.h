#pragma once
#include "type.h"

class SameAs : public Type
{
private:
	std::shared_ptr<Type> t;
public:
	SameAs(std::shared_ptr<Type> t) :
		t(t) {}

	bool isSame(std::shared_ptr<Type> t) const override { return this->t->isSame(t); }
	llvm::Type* getType(llvm::LLVMContext& c) const override { return t->getType(c); }

	bool isFloat() const override { return t->isFloat(); }
	bool isInt() const override { return t->isInt(); }
	bool isSigned() const override { return t->isSigned(); }
	bool isPointer() const override { return t->isPointer(); }
	bool isBool() const override { return t->isBool(); }
	bool isVoid() const override { return t->isVoid(); }
	bool isMut() const override { return t->isMut(); }
	bool isRef() const override { return t->isRef(); }
	bool isArr() const override { return t->isArr(); }
	bool isFuture() const override { return t->isFuture(); }
	unsigned getRank() const override { return t->getRank(); }
	std::shared_ptr<Type> getTypePointedTo() const override { return t->getTypePointedTo(); }
	std::shared_ptr<Type> getUnderlyingType() const override { return getRealType(t); }
};