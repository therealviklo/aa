#include "structtype.h"

bool StructType::isSame(std::shared_ptr<Type> t) const
{
	std::shared_ptr<Type> t2 = getRealType(t);
	if (const StructType* const st = dynamic_cast<const StructType*>(t2.get()))
	{
		return name == st->name;
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
	return llvm::StructType::get(c, types, packed);
}