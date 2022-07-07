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
		if (function.retType->isPtrReturn())
			as.vscope.add(
				"ret",
				{
					ai++,
					std::make_shared<MutType>(function.retType),
					true
				}
			);
		if (function.methodType)
			as.vscope.add(
				"this",
				{
					ai++,
					function.mut ? std::make_shared<MutType>(function.methodType) : function.methodType,
					true
				}
			);
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