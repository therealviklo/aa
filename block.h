#pragma once
#include <vector>
#include "function.h"
#include "statement.h"
#include "scopes.h"

class Block : public Statement
{
private:
	std::vector<std::shared_ptr<Statement>> stmts;
public:
	Block(std::vector<std::shared_ptr<Statement>> stmts) :
		stmts(std::move(stmts)) {}

	void writeStatement(Context& c, Scopes& s) const override;
};