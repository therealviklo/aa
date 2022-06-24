#include "structtype.h"

bool StructType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const StructType* const st = dynamic_cast<const StructType*>(t2.get()))
	{
		if (st->fields.size() != fields.size())
			return false;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (!fields[i]->isSame(st->fields[i]))
				return false;
		}
		return true;
	}
	return false;
}

llvm::Type* StructType::getType(llvm::LLVMContext& c) const
{
	std::vector<llvm::Type*> types;
	for (const auto& i : fields)
	{
		types.push_back(i->getType(c));
	}
	return llvm::StructType::get(c, types);
}