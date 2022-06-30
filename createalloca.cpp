#include "createalloca.h"

llvm::Value* createAlloca(llvm::Type* type, Context& c)
{
	llvm::Function* const parentFunction = c.builder->GetInsertBlock()->getParent();
	llvm::IRBuilder<> startBuilder(&(parentFunction->getEntryBlock()), parentFunction->getEntryBlock().begin());
	return c.builder->CreatePointerCast(startBuilder.CreateAlloca(type), llvm::PointerType::get(*c.c, 0));
}