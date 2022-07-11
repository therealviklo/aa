#include "createalloca.h"

llvm::Value* createAlloca(llvm::Type* type, Context& c)
{
	llvm::Function* const parentFunction = c.builder->GetInsertBlock()->getParent();
	llvm::IRBuilder<> startBuilder(&(parentFunction->getEntryBlock()), parentFunction->getEntryBlock().begin());
	llvm::Value* const mem = startBuilder.CreateAlloca(type, 0u);
	mem->mutateType(llvm::PointerType::get(*c.c, 0));
	return mem;
}