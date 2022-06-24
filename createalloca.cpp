#include "createalloca.h"

llvm::AllocaInst* createAlloca(llvm::Type* type, Context& c)
{
	llvm::Function* const parentFunction = c.builder->GetInsertBlock()->getParent();
	llvm::IRBuilder<> startBuilder(&(parentFunction->getEntryBlock()), parentFunction->getEntryBlock().begin());
	return startBuilder.CreateAlloca(type);
}