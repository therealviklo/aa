#include "variable.h"

llvm::Value* Variable::getValue(Context& c)
{
	return ref ? c.builder->CreateLoad(type->getType(*c.c), var) : var;
}