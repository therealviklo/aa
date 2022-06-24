#include "voidtype.h"

llvm::Type* VoidType::getType(llvm::LLVMContext& c) const
{
	return llvm::Type::getVoidTy(c);
}

unsigned VoidType::getRank() const
{
	return 0;
}