#include "convert.h"

llvm::Value* callPtrReturnConvFun(llvm::Value* mem, const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s)
{
	if (!mem)
	{
		mem = createAlloca(newType->getType(*c.c), c);
		c.tdscope.add({newType, mem});
	}
	std::shared_ptr<Type> oldType = expr.getTypeC(c, s);
	const std::string convFunName = oldType->getName() + "$$" + newType->getName();
	if (s.fscope[convFunName].methodType)
	{
		c.builder->CreateCall(
			s.fscope[convFunName].getFunction(c),
			{mem, expr.getAddress(c, s)}
		);
	}
	else
	{
		if (s.fscope[convFunName].args.size() != 1)
			throw std::runtime_error("Ogiltig konverteringsfunktion");
		if (s.fscope[convFunName].args[0].type->isSame(oldType))
		{
			c.builder->CreateCall(
				s.fscope[convFunName].getFunction(c),
				{mem, expr.getValue(c, s)}
			);
		}
		else
		{
			c.builder->CreateCall(
				s.fscope[convFunName].getFunction(c),
				{mem, expr.getAddress(c, s)}
			);
		}
	}
	return mem;
}

llvm::Value* convert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s)
{
	std::shared_ptr<Type> oldType = expr.getTypeC(c, s);
	if (oldType->isSame(newType))
	{
		return expr.getValue(c, s);
	}
	const std::string convFunName = oldType->getName() + "$$" + newType->getName();
	if (s.fscope.contains(convFunName))
	{
		if (newType->isPtrReturn())
		{
			return c.builder->CreateLoad(
				newType->getType(*c.c),
				callPtrReturnConvFun(nullptr, expr, newType, c, s)
			);
		}
		else
		{
			if (s.fscope[convFunName].methodType)
			{
				return c.builder->CreateCall(
					s.fscope[convFunName].getFunction(c),
					{expr.getAddress(c, s)}
				);
			}
			else
			{
				if (s.fscope[convFunName].args.size() != 1)
					throw std::runtime_error("Ogiltig konverteringsfunktion");
				if (s.fscope[convFunName].args[0].type->isSame(oldType))
				{
					return c.builder->CreateCall(
						s.fscope[convFunName].getFunction(c),
						{expr.getValue(c, s)}
					);
				}
				else
				{
					return c.builder->CreateCall(
						s.fscope[convFunName].getFunction(c),
						{expr.getAddress(c, s)}
					);
				}
			}
		}
	}
	if (newType->isInt())
	{
		if (oldType->isFloat())
		{
			return c.builder->CreateFPToUI(expr.getValue(c, s), newType->getType(*c.c));
		}
		else if (oldType->isInt() || oldType->isBool())
		{
			return c.builder->CreateIntCast(
				expr.getValue(c, s),
				newType->getType(*c.c),
				oldType->isSigned()
			);
		}
	}
	else if (newType->isFloat())
	{
		if (oldType->isInt())
		{
			if (oldType->isSigned())
				return c.builder->CreateSIToFP(expr.getValue(c, s), newType->getType(*c.c));
			else
				return c.builder->CreateUIToFP(expr.getValue(c, s), newType->getType(*c.c));
		}
		else if (oldType->isFloat())
		{
			return c.builder->CreateFPCast(expr.getValue(c, s), newType->getType(*c.c));
		}
	}
	else if (newType->isPointer() || newType->isFunctionPointer())
	{
		if (oldType->isPointer() || oldType->isFunctionPointer())
		{
			return c.builder->CreatePointerCast(expr.getValue(c, s), newType->getType(*c.c));
		}
	}
	else if (newType->isBool())
	{
		if (oldType->isInt())
		{
			return
				c.builder->CreateICmpNE(
					expr.getValue(c, s),
					llvm::ConstantInt::get(oldType->getType(*c.c), 0)
				);
		}
		else if (oldType->isFloat())
		{
			return 
				c.builder->CreateFCmpONE(
					expr.getValue(c, s),
					llvm::ConstantFP::get(oldType->getType(*c.c), 0.0)
				);
		}
		else if (oldType->isPointer())
		{
			llvm::Type* const ptrIntType =
				llvm::IntegerType::get(
					*c.c,
					c.mod->getDataLayout().getTypeAllocSizeInBits(oldType->getType(*c.c))
				);
			return
				c.builder->CreateICmpNE(
					c.builder->CreatePtrToInt(
						expr.getValue(c, s),
						ptrIntType
					),
					llvm::ConstantInt::get(ptrIntType, 0)
				);
		}
	}
	throw std::runtime_error((std::string)"Kan inte konvertera typ " + typeid(*oldType).name() + " till " + typeid(*newType).name());
}

llvm::Value* forceConvert(const Expression& expr, std::shared_ptr<Type> newType, Context& c, Scopes& s)
{
	std::shared_ptr<Type> oldType = expr.getTypeC(c, s);
	if (newType->isInt())
	{
		if (oldType->isPointer())
		{
			return c.builder->CreatePtrToInt(expr.getValue(c, s), newType->getType(*c.c));
		}
	}
	else if (newType->isPointer())
	{
		if (oldType->isInt())
		{
			return c.builder->CreateIntToPtr(expr.getValue(c, s), newType->getType(*c.c));
		}
	}
	return convert(expr, newType, c, s);
}

llvm::Value* Convert::getValue(Context& c, Scopes& s) const
{
	return convert(*expr, newType, c, s);
}

void Convert::getValuePtrReturn(llvm::Value* mem, Context& c, Scopes& s) const
{
	callPtrReturnConvFun(mem, *expr, newType, c, s);
}

bool Convert::canPtrReturn(Context& /*c*/, Scopes& /*s*/) const
{
	return newType->isPtrReturn();
}

std::shared_ptr<Type> Convert::getType(Context& /*c*/, Scopes& /*s*/) const
{
	return newType;
}

llvm::Value* Convert::getAddress(Context& c, Scopes& s) const
{
	std::shared_ptr<Type> oldType = expr->getTypeC(c, s);
	const std::string convFunName = oldType->getName() + "$$" + newType->getName();
	if (!s.fscope.contains(convFunName) || !newType->isPtrReturn())
		return Expression::getAddress(c, s);
	return callPtrReturnConvFun(nullptr, *expr, newType, c, s);
}