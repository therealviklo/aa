#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include "llvm.h"

struct Context;
struct Scopes;

struct Type
{
	Type() = default;
	virtual ~Type() = default;
	
	Type(const Type&) = delete;
	Type& operator=(const Type&) = delete;

	virtual llvm::Type* getType(llvm::LLVMContext& c) const = 0;

	virtual std::string getName() const = 0;
	virtual bool isSame(std::shared_ptr<Type> t) const;
	virtual bool isFloat() const { return false; }
	virtual bool isInt() const { return false; }
	virtual bool isSigned() const { return false; }
	virtual bool isPointer() const { return false; }
	virtual bool isBool() const { return false; }
	virtual bool isVoid() const { return false; }
	virtual bool isMut() const { return false; }
	virtual bool isRef() const { return false; }
	virtual bool isArr() const { return false; }
	virtual bool isFuture() const { return false; }
	virtual bool isFunctionPointer() const { return false; }
	virtual bool isStruct() const { return false; }
	bool isPtrReturn() const { return isArr() || isStruct(); }
	virtual bool isTriviallyDestructible(Scopes& /*s*/) const { return true; }
	virtual unsigned getRank() const { return 0u; }
	virtual std::shared_ptr<Type> getTypePointedTo() const
	{
		throw std::runtime_error("Inte en pekare");
	}
	virtual std::shared_ptr<Type> getUnderlyingType() const
	{
		return nullptr;
	}

	size_t getSize(llvm::Module& mod, llvm::LLVMContext& c) const;

	virtual void destruct(llvm::Value* /*mem*/, Context& /*c*/, Scopes& /*s*/) const {}
};

inline std::shared_ptr<Type> getRealType(std::shared_ptr<Type> t)
{
	if (std::shared_ptr<Type> t2 = t->getUnderlyingType())
		return t2;
	return t;
}