#pragma once
#include "statement.h"
#include "expression.h"
#include "convert.h"

struct IfElse : public Statement
{
	std::unique_ptr<Expression> cond;
	std::unique_ptr<Statement> thenStmt;
	std::unique_ptr<Statement> elseStmt;

	IfElse(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> thenStmt, std::unique_ptr<Statement> elseStmt) :
		cond(std::move(cond)),
		thenStmt(std::move(thenStmt)),
		elseStmt(std::move(elseStmt)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};
