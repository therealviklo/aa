#include "functiontype.h"

bool FunctionType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const FunctionType* const ft = dynamic_cast<const FunctionType*>(t2.get()))
	{
		if (!ft->retType->isSame(retType))
			return false;
		if (ft->argTypes.size() != argTypes.size())
			return false;
		for (size_t i = 0; i < argTypes.size(); i++)
		{
			if (!ft->argTypes[i]->isSame(argTypes[i]))
				return false;
		}
		return true;
	}
	return false;
}

llvm::FunctionType* FunctionType::getFunctionType(llvm::LLVMContext& c) const
{
	std::vector<llvm::Type*> args;
	if (retType->isPtrReturn())
		args.push_back(llvm::PointerType::get(c, 0));
	for (const auto& i : argTypes)
	{
		args.push_back(i->getType(c));
	}
	return 
		llvm::FunctionType::get(
			retType->isPtrReturn()
			? llvm::Type::getVoidTy(c)
			: retType->getType(c),
			args,
			varargs
		);
}

llvm::Type* FunctionType::getType(llvm::LLVMContext& c) const
{
	return getFunctionType(c)->getPointerTo();
}

std::string FunctionType::getName() const
{
	std::string ret = retType->getName();
	ret += "<-(";
	bool firstArg = true;
	for (const auto& i : argTypes)
	{
		if (firstArg)
			firstArg = false;
		else
			ret += ',';
		ret += i->getName();
	}
	ret += ')';
	return ret;
}