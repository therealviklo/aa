#include "module.h"

void genFunction(const Function& function, Context& c, Scopes& s)
{
	c.retType = function.retType;
	llvm::Function* const f = function.getFunction(c);
	if (function.body)
	{
		llvm::BasicBlock* const entryBlock = llvm::BasicBlock::Create(*c.c, "entry", f);
		c.builder->SetInsertPoint(entryBlock);
		Scopes as(&s);
		llvm::Function::arg_iterator ai = f->arg_begin();
		for (const auto& i : function.args)
		{
			as.vscope.add(i.name, {ai++, i.type, false});
		}
		function.body->writeStatement(c, as);
		if (!c.builder->GetInsertBlock()->getTerminator())
			c.builder->CreateRetVoid();
	}
}

void initModule(const std::string& triple, Context& c)
{
	c.mod->setTargetTriple(triple);
}

void genModule(Context& c, Scopes& s)
{
	s.fscope.foreach([&](const Function& f){
		genFunction(f, c, s);
	});
}