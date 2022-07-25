#include "ifelse.h"

void IfElse::writeStatement(Context& c, Scopes& s) const
{
	Scopes as(&s);
	llvm::Value* const boolVal = convert(cond, s.tscope["bool"], c, as);
	llvm::Function* const f = c.builder->GetInsertBlock()->getParent();
	llvm::BasicBlock* const thenBlock = llvm::BasicBlock::Create(*c.c, "", f);
	llvm::BasicBlock* const elseBlock = llvm::BasicBlock::Create(*c.c);
	llvm::BasicBlock* mergeBlock = nullptr;
	c.tdscope.destroy(c, as);
	c.builder->CreateCondBr(boolVal, thenBlock, elseBlock);
	c.builder->SetInsertPoint(thenBlock);
	Scopes aas(&as);
	thenStmt->writeStatement(c, aas);
	bool noMerge = true;
	if (!c.builder->GetInsertBlock()->getTerminator())
	{
		aas.dscope.destroy(c, s);
		if (!mergeBlock)
			mergeBlock = llvm::BasicBlock::Create(*c.c);
		c.builder->CreateBr(mergeBlock);
		noMerge = false;
	}
	f->getBasicBlockList().push_back(elseBlock);
	c.builder->SetInsertPoint(elseBlock);
	Scopes aas2(&as);
	elseStmt->writeStatement(c, aas2);
	if (!c.builder->GetInsertBlock()->getTerminator())
	{
		aas2.dscope.destroy(c, s);
		if (!mergeBlock)
			mergeBlock = llvm::BasicBlock::Create(*c.c);
		c.builder->CreateBr(mergeBlock);
		noMerge = false;
	}
	if (!noMerge)
	{
		f->getBasicBlockList().push_back(mergeBlock);
		c.builder->SetInsertPoint(mergeBlock);
		as.dscope.destroy(c, s);
	}
}