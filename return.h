#pragma once
#include <memory>
#include "expression.h"
#include "statement.h"
#include "convert.h"

struct Return : public Statement
{
	std::unique_ptr<Expression> val;

	Return(std::unique_ptr<Expression> val) :
		val(std::move(val)) {}
	
	void writeStatement(Context& c, Scopes& s) const override;
};