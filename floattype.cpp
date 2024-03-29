#include "floattype.h"

bool FloatType::isSame(std::shared_ptr<Type> t) const
{
	if (const FloatType* const ft = dynamic_cast<const FloatType*>(t.get()))
	{
		return ft->fkind == fkind;
	}
	return false;
}

llvm::Type* FloatType::getType(llvm::LLVMContext& c) const
{
	switch (fkind)
	{
		case FloatKind::f16:
		return llvm::Type::getHalfTy(c);
		case FloatKind::f32:
		return llvm::Type::getFloatTy(c);
		case FloatKind::f64:
		return llvm::Type::getDoubleTy(c);
	}
}

std::string FloatType::getName() const
{
	switch (fkind)
	{
		case FloatKind::f16:
		return "f16";
		case FloatKind::f32:
		return "f32";
		case FloatKind::f64:
		return "f64";
	}
}

unsigned FloatType::getRank() const
{
	switch (fkind)
	{
		case FloatKind::f16:
		return 1016;
		case FloatKind::f32:
		return 1032;
		case FloatKind::f64:
		return 1064;
	}
}