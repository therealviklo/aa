#pragma once
#include "statement.h"
#include "expression.h"
#include "convert.h"

struct While : public Statement
{
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> stmt;

	While(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> stmt) :
		cond(std::move(cond)),
		stmt(std::move(stmt)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};