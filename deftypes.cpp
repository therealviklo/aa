#include "deftypes.h"

void addDefaultTypes(Context& c, Scope<std::shared_ptr<Type>>& scope)
{
	scope.add("void", std::make_shared<VoidType>());
	scope.add("u8", std::make_shared<IntegerType>(8, true));
	scope.add("u16", std::make_shared<IntegerType>(16, true));
	scope.add("u32", std::make_shared<IntegerType>(32, true));
	scope.add("u64", std::make_shared<IntegerType>(64, true));
	scope.add("umax", std::make_shared<IntegerType>(64, true));
	scope.add("usize", std::make_shared<IntegerType>(c.mod->getDataLayout().getPointerSizeInBits(), true));
	scope.add("uptr", std::make_shared<IntegerType>(c.mod->getDataLayout().getPointerSizeInBits(), true));
	scope.add("i8", std::make_shared<IntegerType>(8, false));
	scope.add("i16", std::make_shared<IntegerType>(16, false));
	scope.add("i32", std::make_shared<IntegerType>(32, false));
	scope.add("i64", std::make_shared<IntegerType>(64, false));
	scope.add("imax", std::make_shared<IntegerType>(64, false));
	scope.add("isize", std::make_shared<IntegerType>(c.mod->getDataLayout().getPointerSizeInBits(), false));
	scope.add("iptr", std::make_shared<IntegerType>(c.mod->getDataLayout().getPointerSizeInBits(), false));
	scope.add("f16", std::make_shared<FloatType>(FloatKind::f16));
	scope.add("f32", std::make_shared<FloatType>(FloatKind::f32));
	scope.add("f64", std::make_shared<FloatType>(FloatKind::f64));
	scope.add("bool", std::make_shared<BoolType>());
}