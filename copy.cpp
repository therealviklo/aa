#include "copy.h"

void copy(std::shared_ptr<Expression> from, llvm::Value* to, std::shared_ptr<Type> type, Context& c, Scopes& s)
{
	std::shared_ptr<Expression> convfrom =
		type->isSame(from->getTypeC(c, s)) ?
		from :
		std::make_shared<Convert>(from, type);
	if (convfrom->canPtrReturn())
	{
		convfrom->getValuePtrReturn(to, c, s);
	}
	else if (type->isStruct())
	{
		const std::string convFunName = type->getName() + "$$" + type->getName();
		if (s.fscope.contains(convFunName))
		{
			c.builder->CreateCall(
				s.fscope[convFunName].getFunction(c),
				{to, convfrom->getAddress(c, s)}
			);
		}
		else if (!s.fscope.contains("~" + type->getName()))
		{
			c.builder->CreateStore(convfrom->getValue(c, s), to);
		}
		else
		{
			throw std::runtime_error("Kan inte kopiera typ");
		}
	}
	// else if (type->isArr())
	// {
	// 	const size_t arrSize = type->getArrSize();
	// 	for (size_t i = 0; i < arrSize; i++)
	// 	{
	// 		llvm::Value* const left = c.builder->CreateGEP(
	// 			type->getType(*c.c),
	// 			convfrom->getAddress(c, s),
	// 			{
	// 				llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), 0),
	// 				llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), i)
	// 			}
	// 		);
	// 		std::shared_ptr<Expression> right = std::make_shared<Subscript>(
	// 			convfrom,
	// 			std::make_shared<Name>(std::to_string(i) + "u")
	// 		);
	// 		copy(right, left, type->getTypePointedTo(), c, s);
	// 	}
	// }
	else
	{
		c.builder->CreateStore(convfrom->getValue(c, s), to);
	}
}