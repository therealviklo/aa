#pragma once
#include "statement.h"
#include "expression.h"
#include "convert.h"

struct If : public Statement
{
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> thenStmt;

	If(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> thenStmt) :
		cond(std::move(cond)),
		thenStmt(std::move(thenStmt)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};
