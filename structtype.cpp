#include "structtype.h"

bool StructType::isSame(std::shared_ptr<Type> t) const
{
	if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
	{
		return name == st->name;
	}
	return false;
}

bool StructType::isTriviallyDestructible(Scopes& s) const
{
	if (s.fscope.contains("~" + name))
		return false;
	for (size_t i = 0; i < fields.size(); i++)
	{
		if (!fields[i]->isTriviallyDestructible(s))
			return false;
	}
	return true;
}

bool StructType::isTriviallyCopyable(Scopes& s) const
{
	const std::string convFunName = getConvFunName(*this, *this);
	if (s.fscope.contains(convFunName))
		return false;
	if (s.fscope.contains("~" + name))
		return false;
	for (size_t i = 0; i < fields.size(); i++)
	{
		if (!fields[i]->isTriviallyCopyable(s))
			return false;
	}
	return true;
}

llvm::Type* StructType::getType(llvm::LLVMContext& c) const
{
	std::vector<llvm::Type*> types;
	for (const auto& i : fields)
	{
		types.push_back(i->getType(c));
	}
	return llvm::StructType::get(c, types, packed);
}

llvm::Value* StructType::getField(llvm::Value* mem, size_t num, Context& c, Scopes& s) const
{
	return c.builder->CreateGEP(
		getType(*c.c),
		mem,
		{
			llvm::ConstantInt::get(s.tscope["u32"]->getType(*c.c), 0),
			llvm::ConstantInt::get(s.tscope["u32"]->getType(*c.c), num)
		}
	);
}

void StructType::destruct(llvm::Value* mem, Context& c, Scopes& s) const
{
	for (size_t i = 0; i < fields.size(); i++)
	{
		fields[i]->destruct(getField(mem, i, c, s), c, s);
	}
	if (s.fscope.contains("~" + name))
		Name("~" + name).createCall({mem}, c, s);
}