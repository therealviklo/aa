#include "while.h"

void While::writeStatement(Context& c, Scopes& s) const
{
	llvm::Function* const f = c.builder->GetInsertBlock()->getParent();
	Scopes as(&s);
	llvm::BasicBlock* const check = llvm::BasicBlock::Create(*c.c, "", f);
	c.builder->CreateBr(check);
	c.builder->SetInsertPoint(check);
	llvm::Value* const boolVal = convert(*cond, (*s.tscope)["bool"], c, s);
	llvm::BasicBlock* const block = llvm::BasicBlock::Create(*c.c, "", f);
	llvm::BasicBlock* const mergeBlock = llvm::BasicBlock::Create(*c.c);
	c.builder->CreateCondBr(boolVal, block, mergeBlock);
	c.builder->SetInsertPoint(block);
	Scopes aas(&as);
	stmt->writeStatement(c, aas);
	if (!c.builder->GetInsertBlock()->getTerminator())
		c.builder->CreateBr(check);
	f->getBasicBlockList().push_back(mergeBlock);
	c.builder->SetInsertPoint(mergeBlock);
}