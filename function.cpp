#include "function.h"

llvm::Function* Function::getFunction(Context& c) const
{
	if (llvm::Function* const f = c.mod->getFunction(name))
		return f;
	std::vector<llvm::Type*> argTypes;
	if (methodType)
	{
		if (mut)
			argTypes.push_back(
				std::make_shared<PointerType>(
					std::make_shared<MutType>(methodType)
				)->getType(*c.c)
			);
		else
			argTypes.push_back(
				std::make_shared<PointerType>(
					methodType
				)->getType(*c.c)
			);
	}
	for (const auto& i : args)
	{
		argTypes.push_back(i.type->getType(*c.c));
	}
	llvm::FunctionType* const ftype = llvm::FunctionType::get(retType->getType(*c.c), argTypes, varargs);
	return llvm::Function::Create(ftype, llvm::Function::ExternalLinkage, name, *c.mod);
}

std::shared_ptr<FunctionType> Function::getFunctionType() const
{
	std::vector<std::shared_ptr<Type>> argTypes;
	if (methodType)
	{
		if (mut)
			argTypes.push_back(
				std::make_shared<PointerType>(
					std::make_shared<MutType>(methodType)
				)
			);
		else
			argTypes.push_back(
				std::make_shared<PointerType>(
					methodType
				)
			);
	}
	for (const auto& i : args)
	{
		argTypes.push_back(i.type);
	}
	return std::make_shared<FunctionType>(retType, std::move(argTypes), varargs);
}