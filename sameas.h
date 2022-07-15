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

	std::string getName() const override { return t->getName(); }
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
	bool isFunctionPointer() const override { return t->isFunctionPointer(); }
	bool isStruct() const override { return t->isStruct(); }
	bool isTriviallyDestructible(Scopes& s) const override { return t->isTriviallyDestructible(s); }
	std::shared_ptr<Type> getTypePointedTo() const override { return t->getTypePointedTo(); }
	std::shared_ptr<Type> getUnderlyingType() const override { return getRealType(t); }

	void destruct(llvm::Value* mem, Context& c, Scopes& s) const override
	{
		t->destruct(mem, c, s);
	}
};