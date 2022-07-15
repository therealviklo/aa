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
	else
	{
		c.builder->CreateStore(convfrom->getValue(c, s), to);
	}
}