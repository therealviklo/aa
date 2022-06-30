#pragma once
#include "statement.h"
#include "expression.h"
#include "convert.h"

struct IfElse : public Statement
{
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> thenStmt;
	std::shared_ptr<Statement> elseStmt;

	IfElse(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> thenStmt, std::shared_ptr<Statement> elseStmt) :
		cond(std::move(cond)),
		thenStmt(std::move(thenStmt)),
		elseStmt(std::move(elseStmt)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};
