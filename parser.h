#pragma once
#include <fstream>
#include <filesystem>
#include <set>
#include "lexer.h"
#include "scopes.h"
#include "function.h"
#include "name.h"
#include "typenamepair.h"
#include "block.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "mod.h"
#include "assign.h"
#include "return.h"
#include "funccall.h"
#include "if.h"
#include "ifelse.h"
#include "while.h"
#include "convert.h"
#include "pointertype.h"
#include "addrof.h"
#include "deref.h"
#include "stringlit.h"
#include "negate.h"
#include "sizeof.h"
#include "eq.h"
#include "neq.h"
#include "lt.h"
#include "gt.h"
#include "lte.h"
#include "gte.h"
#include "and.h"
#include "or.h"
#include "xor.h"
#include "not.h"
#include "bitcast.h"
#include "arraytype.h"
#include "subscript.h"
#include "structtype.h"
#include "dotop.h"
#include "futuretype.h"
#include "functiontype.h"
#include "createstruct.h"
#include "voidtype.h"
#include "inplaceconstruct.h"

namespace fs = std::filesystem;

class Parser
{
private:
	std::string filename;
	Lexer lexer;
	std::shared_ptr<std::set<fs::path>> visitedFiles;
public:
	Parser(const std::string& filename, std::shared_ptr<std::set<fs::path>> visitedFiles);

	void parseFile(Scopes& s);
	void parseTypeNamePair(TypeNamePair&& tnp, std::shared_ptr<Type> methodType, bool mut, Scopes& s);
	void parseFunctionArgs(TypeNamePair&& tnp, std::shared_ptr<Type> methodType, bool mut, Scopes& s);
	std::shared_ptr<Statement> parseStatement(Scopes& s);
	std::shared_ptr<Statement> parseCompoundStatement(Scopes& s);
	std::shared_ptr<Expression> parseExpression(int lvl, Scopes& s);
	std::shared_ptr<Expression> parseExpressionRight(int lvl, std::shared_ptr<Expression> left, Scopes& s);
	std::vector<std::shared_ptr<Expression>> parseFuncCallArgs(Scopes& s);
	std::shared_ptr<Statement> parseReturnStatement(Scopes& s);
	std::shared_ptr<Statement> parseIfStatement(Scopes& s);
	std::shared_ptr<Statement> parseWhileStatement(Scopes& s);
	std::shared_ptr<Type> parseType(const std::string& name, Scopes& s);
	std::string parseEscapedCharacter();
	void parseTypeDecl(Scopes& s);
	void parseStruct(std::string sname, bool packed, Scopes& s);
};