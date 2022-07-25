#pragma once
#include "createstruct.h"

class InPlaceConstruct : public Expression
{
private:
	std::shared_ptr<Expression> mem;
	CreateStruct cs;
public:
	InPlaceConstruct(
		std::shared_ptr<Expression> mem,
		std::shared_ptr<Type> type,
		std::vector<std::shared_ptr<Expression>> args
	) :
		mem(std::move(mem)),
		cs(std::move(type), std::move(args)) {}

	llvm::Value* get(Context& c, Scopes& s) const override;
	std::shared_ptr<Type> getType(Context& c, Scopes& s) const override;
};