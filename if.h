#pragma once
#include "statement.h"
#include "expression.h"
#include "convert.h"

struct If : public Statement
{
	std::unique_ptr<Expression> cond;
	std::unique_ptr<Statement> thenStmt;

	If(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> thenStmt) :
		cond(std::move(cond)),
		thenStmt(std::move(thenStmt)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};
