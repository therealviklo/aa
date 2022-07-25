#include "if.h"

void If::writeStatement(Context& c, Scopes& s) const
{
	Scopes as(&s);
	llvm::Value* const boolVal = convert(cond, s.tscope["bool"], c, as);
	llvm::Function* const f = c.builder->GetInsertBlock()->getParent();
	llvm::BasicBlock* const thenBlock = llvm::BasicBlock::Create(*c.c, "", f);
	llvm::BasicBlock* const mergeBlock = llvm::BasicBlock::Create(*c.c);
	c.tdscope.destroy(c, as);
	c.builder->CreateCondBr(boolVal, thenBlock, mergeBlock);
	c.builder->SetInsertPoint(thenBlock);
	Scopes aas(&as);
	thenStmt->writeStatement(c, aas);
	if (!c.builder->GetInsertBlock()->getTerminator())
	{
		aas.dscope.destroy(c, s);
		c.builder->CreateBr(mergeBlock);
	}
	f->getBasicBlockList().push_back(mergeBlock);
	c.builder->SetInsertPoint(mergeBlock);
	as.dscope.destroy(c, s);
}