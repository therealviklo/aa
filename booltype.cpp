#include "booltype.h"

llvm::Type* BoolType::getType(llvm::LLVMContext& c) const
{
	return llvm::IntegerType::get(c, 1);
}

unsigned BoolType::getRank() const
{
	return 1;
}