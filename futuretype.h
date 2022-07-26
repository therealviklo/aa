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
	
	std::string getName() const override { return name; }
	bool isSame(std::shared_ptr<Type> t) const override;
	bool isFuture() const override { return !tscope.contains(name); }
	bool isStruct() const override { return true; }
	bool isTriviallyDestructible(Scopes& s) const override
	{
		return get()->isTriviallyDestructible(s);
	}
	bool isTriviallyCopyable(Scopes& s) const override
	{
		return get()->isTriviallyCopyable(s);
	}
	llvm::Type* getType(llvm::LLVMContext& c) const override { return get()->getType(c); }
	std::shared_ptr<Type> getRealType() const override { return ::getRealType(get()); }
	std::shared_ptr<Type> getUnderlyingType() const override { return ::getUnderlyingType(get()); }
	std::shared_ptr<Type> getValueType() const override { return ::getValueType(get()); }

	void destruct(llvm::Value* mem, Context& c, Scopes& s) const override;
};