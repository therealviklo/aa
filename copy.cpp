#include "copy.h"

void copy(std::shared_ptr<Expression> from, llvm::Value* to, std::shared_ptr<Type> type, Context& c, Scopes& s)
{
	std::shared_ptr<Expression> convfrom =
		!type->isRef() && type->isSameValue(from->getTypeC(c, s)) ?
		from :
		std::make_shared<Convert>(from, type);
	if (type->isRef())
	{
		c.builder->CreateStore(from->getAddress(c, s), to);
	}
	else if (convfrom->canPtrReturn(c, s))
	{
		convfrom->getValuePtrReturn(to, c, s);
	}
	else if (type->isStruct())
	{
		if (type->isTriviallyCopyable(s))
		{
			c.builder->CreateStore(convfrom->getValue(c, s), to);
		}
		else
		{
			const std::string convFunName = getConvFunName(*type, *type);
			if (s.fscope.contains(convFunName))
			{
				c.builder->CreateCall(
					s.fscope[convFunName].getFunction(c),
					{to, convfrom->getAddress(c, s)}
				);
			}
			else if (s.fscope.contains("~" + type->getName()))
			{
				throw std::runtime_error("Kan inte kopiera typ");
			}
			else
			{
				std::shared_ptr<Type> vt = getValueType(type);
				if (const StructType* const st = dynamic_cast<const StructType*>(vt.get()))
				{
					for (size_t i = 0; i < st->fields.size(); i++)
					{
						copy(
							std::make_shared<StructMemByNumber>(
								convfrom->getAddress(c, s),
								type,
								i
							),
							std::make_shared<StructMemByNumber>(
								to,
								type,
								i
							)->getAddress(c, s),
							st->fields[i],
							c,
							s
						);
					}
				}
				else
				{
					throw std::runtime_error("Förväntade structtyp");
				}
			}
		}
	}
	else if (type->isArr())
	{
		const size_t arrSize = type->getArrSize();
		for (size_t i = 0; i < arrSize; i++)
		{
			llvm::Value* const left = c.builder->CreateGEP(
				type->getType(*c.c),
				convfrom->getAddress(c, s),
				{
					llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), 0),
					llvm::ConstantInt::get(s.tscope["usize"]->getType(*c.c), i)
				}
			);
			std::shared_ptr<Expression> right = std::make_shared<Subscript>(
				convfrom,
				std::make_shared<Name>(std::to_string(i) + "u")
			);
			copy(right, left, type->getTypePointedTo(), c, s);
		}
	}
	else
	{
		c.builder->CreateStore(convfrom->getValue(c, s), to);
	}
}