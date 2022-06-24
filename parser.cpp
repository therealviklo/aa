#include "parser.h"

constexpr int level(const std::string& op)
{
	if (op == "=")
		return 1;
	if (op == "||")
		return 2;
	if (op == "^^")
		return 4;
	if (op == "&&")
		return 6;
	if (op == "==")
		return 8;
	if (op == "!=")
		return 8;
	if (op == "<")
		return 10;
	if (op == ">")
		return 10;
	if (op == "<=")
		return 10;
	if (op == ">=")
		return 10;
	if (op == "+")
		return 12;
	if (op == "-")
		return 12;
	if (op == "*")
		return 14;
	if (op == "/")
		return 14;
	if (op == "%")
		return 14;
	if (op == "&")
		return 15;
	if (op == "(")
		return 16;
	if (op == "[")
		return 16;
	if (op == "[[")
		return 16;
	if (op == "!")
		return 16;
	if (op == ".")
		return 18;
	return 0;
}

constexpr int prelevel(const std::string& op)
{
	if (op == "[")
		return 15;
	if (op == "+")
		return 15;
	if (op == "-")
		return 15;
	if (op == "!")
		return 15;
	return level(op);
}

constexpr bool checkLevel(int lvl, int currlvl)
{
	if (currlvl % 2 == 0)
		return lvl > currlvl;
	else
		return lvl >= currlvl;
}

Parser::Parser(const std::string& filename, std::shared_ptr<std::set<fs::path>> visitedFiles) :
	filename(filename),
	lexer([&](){
		std::string contents(fs::file_size(filename), '\0');
		{
			std::ifstream fs;
			fs.exceptions(std::ios::badbit | std::ios::failbit);
			fs.open(filename, std::ios::binary);
			fs.read(contents.data(), contents.size());
		}
		return Lexer(std::move(contents));
	}()),
	visitedFiles(visitedFiles) {}

void Parser::parseFile(Scopes& s)
{
	while (!lexer.atEnd())
	{
		if (lexer.tryReadName("type"))
		{
			parseTypeDecl(s);
		}
		else if (lexer.tryReadName("include"))
		{
			if (!lexer.tryRead("\""))
				throw std::runtime_error("Förväntade \"");
			std::string str;
			char32_t ch;
			while ((ch = lexer.getUnicodeChar()) != U'"')
			{
				if (ch == U'\\')
					str += parseEscapedCharacter();
				else
					str += utf32CharToUtf8(ch);
			}
			if (!lexer.tryRead(";"))
				throw std::runtime_error("Förväntade \";\"");
			const fs::path newFilename = fs::canonical(fs::path(filename).parent_path() / str);
			if (!visitedFiles->contains(newFilename))
			{
				visitedFiles->emplace(newFilename);
				Parser newp(newFilename, visitedFiles);
				newp.parseFile(s);
			}
		}
		else
		{
			std::string token = lexer.getRegex(nameregex);
			if (!s.tscope->contains(token))
				throw std::runtime_error("Oväntad token \"" + token + '\"');
			std::shared_ptr<Type> type = parseType(token, s);
			std::string name = lexer.getRegex(nameregex);
			if (name.empty())
				throw std::runtime_error("Förväntade namn");
			parseTypeNamePair(TypeNamePair(type, Name(std::move(name))), s);
		}
	}
}

void Parser::parseTypeNamePair(TypeNamePair&& tnp, Scopes& s)
{
	if (lexer.tryRead("("))
	{
		parseFunctionArgs(std::move(tnp), s);
	}
	else throw std::runtime_error("Inte implementerat än ;)");
}

void Parser::parseFunctionArgs(TypeNamePair&& tnp, Scopes& s)
{
	std::vector<FuncArg> args;
	bool varargs = false;
	if (!lexer.tryRead(")"))
	{
		do
		{
			if (lexer.tryRead("..."))
			{
				varargs = true;
				break;
			}
			const std::string token = lexer.getRegex(nameregex);
			if (!s.tscope->contains(token))
				throw std::runtime_error("Oväntad token \"" + token + '\"');
			std::shared_ptr<Type> type = parseType(token, s);
			const std::string name = lexer.getRegex(nameregex);
			if (name.empty())
				throw std::runtime_error("Förväntade namn");
			args.emplace_back(FuncArg{type, name});
		} while (lexer.tryRead(","));
		if (!lexer.tryRead(")"))
			throw std::runtime_error("Förväntade \")\"");
	}
	if (lexer.tryRead(";"))
	{
		s.fscope->add(tnp.name.name, Function{tnp.name.name, std::move(args), varargs, tnp.type, nullptr});
	}
	else
	{
		Scopes as(&s);
		std::unique_ptr<Statement> body = parseStatement(as);
		s.fscope->add(tnp.name.name, Function{tnp.name.name, std::move(args), varargs, tnp.type, std::move(body)});
	}
}

std::unique_ptr<Statement> Parser::parseStatement(Scopes& s)
{
	if (lexer.tryRead("{"))
	{
		return parseCompoundStatement(s);
	}
	else if (lexer.tryReadName("return"))
	{
		return parseReturnStatement(s);
	}
	else if (lexer.tryReadName("if"))
	{
		return parseIfStatement(s);
	}
	else if (lexer.tryReadName("while"))
	{
		return parseWhileStatement(s);
	}
	else
	{
		std::unique_ptr<Statement> expr = parseExpression(0, s);
		if (!lexer.tryRead(";"))
			throw std::runtime_error("Förväntade \";\"");
		return expr;
	}
}

std::unique_ptr<Statement> Parser::parseCompoundStatement(Scopes& s)
{
	std::vector<std::unique_ptr<Statement>> stmts;
	while (!lexer.tryRead("}"))
	{
		stmts.push_back(parseStatement(s));
	}
	return std::make_unique<Block>(std::move(stmts));
}

std::unique_ptr<Expression> Parser::parseExpression(int lvl, Scopes& s)
{
#define mapPreUnOp(str, Op) if (lexer.tryRead(str)) \
	{ \
		std::unique_ptr<Expression> expr = parseExpression(prelevel(str), s); \
		return parseExpressionRight( \
			lvl, \
			std::make_unique<Op>(std::move(expr)), \
			s \
		); \
	}
	mapPreUnOp("&", AddrOf);
	mapPreUnOp("-", Neg);
	mapPreUnOp("!", Not);
	if (lexer.tryRead("+"))
	{
		std::unique_ptr<Expression> expr = parseExpression(prelevel("+"), s);
		return parseExpressionRight(
			lvl,
			std::move(expr),
			s
		);
	}
	if (lexer.tryRead("("))
	{
		std::unique_ptr<Expression> expr = parseExpression(0, s);
		if (!lexer.tryRead(")"))
			throw std::runtime_error("Förväntade \")\"");
		return parseExpressionRight(
			lvl,
			std::move(expr),
			s
		);
	}
	if (lexer.tryRead("[["))
	{
		std::string tname = lexer.getRegex(nameregex);
		if (!s.tscope->contains(tname))
			throw std::runtime_error("Förväntade typ");
		std::shared_ptr<Type> type = parseType(tname, s);
		if (!lexer.tryRead("]]"))
			throw std::runtime_error("Förväntade \"]]\"");
		std::unique_ptr<Expression> expr = parseExpression(prelevel("[["), s);
		return parseExpressionRight(
			lvl,
			std::make_unique<BitCast>(std::move(expr), type),
			s
		);
	}
	if (lexer.tryRead("["))
	{
		std::string tname = lexer.getRegex(nameregex);
		if (!s.tscope->contains(tname))
			throw std::runtime_error("Förväntade typ");
		std::shared_ptr<Type> type = parseType(tname, s);
		if (!lexer.tryRead("]"))
			throw std::runtime_error("Förväntade \"]\"");
		std::unique_ptr<Expression> expr = parseExpression(prelevel("["), s);
		return parseExpressionRight(
			lvl,
			std::make_unique<Convert>(std::move(expr), type),
			s
		);
	}
	if (lexer.tryRead("l\""))
	{
		std::string str;
		char32_t ch;
		while ((ch = lexer.getUnicodeChar()) != U'"')
		{
			if (ch == U'\\')
				str += parseEscapedCharacter();
			else
				str += utf32CharToUtf8(ch);
		}
		return parseExpressionRight(
			lvl,
			std::make_unique<StringLit>(
				std::move(str)
			),
			s
		);
	}
	if (lexer.tryRead("l'"))
	{
		std::string str;
		char32_t ch;
		while ((ch = lexer.getUnicodeChar()) != U'\'')
		{
			if (ch == U'\\')
				str += parseEscapedCharacter();
			else
				str += utf32CharToUtf8(ch);
		}
		if (str.size() != 1)
			throw std::runtime_error("Teckenkonstant måste bestå av ett enda enwordigt tecken.");
		std::stringstream ss;
		ss << static_cast<std::uint32_t>(str[0]) << "u8";
		return parseExpressionRight(
			lvl,
			std::make_unique<Name>(
				ss.str()
			),
			s
		);
	}
	if (lexer.tryReadName("sizeof"))
	{
		if (!lexer.tryRead("("))
			throw std::runtime_error("Förväntade \"(\"");
		std::string name = lexer.getRegex(nameregex);
		if (name.empty())
			throw std::runtime_error("Förväntade typ");
		std::shared_ptr<Type> t = parseType(name, s);
		if (!lexer.tryRead(")"))
			throw std::runtime_error("Förväntade \")\"");
		return parseExpressionRight(
			lvl,
			std::make_unique<SizeOf>(t),
			s
		);
	}

	std::string operand = lexer.getRegex(nameregex);
	if (operand.empty())
		throw std::runtime_error("Förväntade namn");
	if (s.tscope->contains(operand))
	{
		std::shared_ptr<Type> type = parseType(operand, s);
		std::string name = lexer.getRegex(nameregex);
		if (name.empty())
			throw std::runtime_error("Förväntade namn");
		return parseExpressionRight(
			lvl,
			std::make_unique<TypeNamePair>(
				type,
				Name(std::move(name))
			),
			s
		);
	}
	else
	{
		return parseExpressionRight(lvl, std::make_unique<Name>(std::move(operand)), s);
	}
}

std::unique_ptr<Expression> Parser::parseExpressionRight(int lvl, std::unique_ptr<Expression> left, Scopes& s)
{
#define mapBinOp(str, Op) if (checkLevel(level(str), lvl) && lexer.tryRead(str)) \
		{ \
			return parseExpressionRight( \
				lvl, \
				std::make_unique<Op>( \
					std::move(left), \
					parseExpression( \
						level(str), \
						s \
					) \
				), \
				s \
			); \
		}
#define mapPostUnOp(str, Op) if (checkLevel(level(str), lvl) && lexer.tryRead(str)) \
		{ \
			return parseExpressionRight( \
				lvl, \
				std::make_unique<Op>( \
					std::move(left) \
				), \
				s \
			); \
		}
	mapBinOp("||", Or)
	mapBinOp("^^", Xor)
	mapBinOp("&&", And)
	mapBinOp("==", Eq)
	mapBinOp("!=", Neq)
	mapBinOp("<=", Lte)
	mapBinOp(">=", Gte)
	mapBinOp("=", Assign)
	mapBinOp("<", Lt)
	mapBinOp(">", Gt)
	mapBinOp("+", Add)
	mapBinOp("-", Sub)
	mapBinOp("*", Mul)
	mapBinOp("/", Div)
	mapBinOp("%", Mod)
	mapPostUnOp("!", Deref)

	if (checkLevel(level("."), lvl) && lexer.tryRead("."))
	{
		std::string fieldname = lexer.getRegex(nameregex);
		if (fieldname.empty())
			throw std::runtime_error("Förväntade namn");
		return parseExpressionRight(
			lvl,
			std::make_unique<DotOp>(
				std::move(left),
				std::move(fieldname)
			),
			s
		);
	}
	
	if (checkLevel(level("("), lvl) && lexer.tryRead("("))
	{
		auto args = parseFuncCallArgs(s);
		return parseExpressionRight(
			lvl,
			std::make_unique<FuncCall>(
				std::move(left),
				std::move(args)
			),
			s
		);
	}
	if (checkLevel(level("["), lvl) && lexer.tryRead("["))
	{
		std::unique_ptr<Expression> expr = parseExpression(0, s);
		if (!lexer.tryRead("]"))
			throw std::runtime_error("Förväntade \"]\"");
		return parseExpressionRight(
			lvl,
			std::make_unique<Subscript>(
				std::move(left),
				std::move(expr)
			),
			s
		);
	}

	return left;
}

std::vector<std::unique_ptr<Expression>> Parser::parseFuncCallArgs(Scopes& s)
{
	std::vector<std::unique_ptr<Expression>> args;
	if (!lexer.tryRead(")"))
	{
		do
		{
			args.push_back(parseExpression(0, s));
		} while (lexer.tryRead(","));
		if (!lexer.tryRead(")"))
			throw std::runtime_error("Förväntade \")\"");
	}
	return args;
}

std::unique_ptr<Statement> Parser::parseReturnStatement(Scopes& s)
{
	if (lexer.tryRead(";"))
		return std::make_unique<Return>(nullptr);
	std::unique_ptr<Expression> expr = parseExpression(0, s);
	if (!lexer.tryRead(";"))
		throw std::runtime_error("Förväntade \";\"");
	return std::make_unique<Return>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseIfStatement(Scopes& s)
{
	Scopes as(&s);
	if (!lexer.tryRead("("))
		throw std::runtime_error("Förväntade \"(\"");
	std::unique_ptr<Expression> cond = parseExpression(0, as);
	if (!lexer.tryRead(")"))
		throw std::runtime_error("Förväntade \")\"");
	Scopes aas(&as);
	std::unique_ptr<Statement> thenStmt = parseStatement(aas);
	if (lexer.tryReadName("else"))
	{
		Scopes aas2(&as);
		std::unique_ptr<Statement> elseStmt = parseStatement(aas2);
		return std::make_unique<IfElse>(std::move(cond), std::move(thenStmt), std::move(elseStmt));
	}
	else
	{
		return std::make_unique<If>(std::move(cond), std::move(thenStmt));
	}
}

std::unique_ptr<Statement> Parser::parseWhileStatement(Scopes& s)
{
	Scopes as(&s);
	if (!lexer.tryRead("("))
		throw std::runtime_error("Förväntade \"(\"");
	std::unique_ptr<Expression> cond = parseExpression(0, as);
	if (!lexer.tryRead(")"))
		throw std::runtime_error("Förväntade \")\"");
	Scopes aas(&as);
	std::unique_ptr<Statement> stmt = parseStatement(aas);
	return std::make_unique<While>(std::move(cond), std::move(stmt));
}

std::shared_ptr<Type> Parser::parseType(const std::string& name, Scopes& s)
{
	std::shared_ptr<Type> type = (*s.tscope)[name];
	while (true)
	{
		if (lexer.tryReadName("mut"))
		{
			if (type->isMut())
				throw std::runtime_error("Dubbel mut är inte tillåtet");
			type = std::make_shared<MutType>(type);
		}
		else if (lexer.tryRead("*"))
		{
			if (type->isVoid())
				type = std::make_shared<PointerType>((*s.tscope)["u8"]);
			else
				type = std::make_shared<PointerType>(type);
		}
		else if (lexer.tryRead("["))
		{
			const std::string numstr = lexer.getRegex(numregex);
			if (numstr.empty())
				throw std::runtime_error("Förväntade tal");
			if (!lexer.tryRead("]"))
				throw std::runtime_error("Förväntade \"]\"");
			const unsigned long long num = std::stoull(numstr);
			type = std::make_shared<ArrayType>(type, num);
		}
		else break;
	}
	return type;
}

std::string Parser::parseEscapedCharacter()
{
	const char32_t c = lexer.getUnicodeChar();
	switch (c)
	{
		case U'\\':
			return "\\";
		case U'"':
			return "\"";
		case U'\'':
			return "'";
		case U'n':
			return "\n";
		case U't':
			return "\t";
		case U'e':
			return "\e";
		case U'r':
			return "\r";
		case U'0':
			return "\0";
	}
	throw std::runtime_error("Okänd escapesekvens");
}

void Parser::parseTypeDecl(Scopes& s)
{
	std::string name = lexer.getRegex(nameregex);
	if (name.empty())
		throw std::runtime_error("Förväntade namn");
	if (lexer.tryRead(";"))
	{
		s.tscope->add(name, std::make_shared<FutureType>(*s.tscope, std::move(name)));
	}
	else if (lexer.tryRead("="))
	{
		std::string oname = lexer.getRegex(nameregex);
		if (oname.empty())
			throw std::runtime_error("Förväntade namn");
		std::shared_ptr<Type> t = parseType(oname, s);
		if (!lexer.tryRead(";"))
			throw std::runtime_error("Förväntade \";\"");
		s.tscope->add(name, t);
	}
	else
	{
		if (!lexer.tryRead("{"))
			throw std::runtime_error("Förväntade \"{\"");
		s.tscope->add(name, std::make_shared<FutureType>(*s.tscope, name));
		parseStruct(std::move(name), s);
	}
}

void Parser::parseStruct(std::string sname, Scopes& s)
{
	std::vector<std::shared_ptr<Type>> fields;
	std::map<std::string, size_t> fieldnames;
	while (!lexer.tryRead("}"))
	{
		std::string tname = lexer.getRegex(nameregex);
		if (tname.empty())
			throw std::runtime_error("Förväntade namn");
		std::shared_ptr<Type> t = parseType(tname, s);
		std::string name = lexer.getRegex(nameregex);
		if (!name.empty())
		{
			fieldnames.emplace(name, fields.size());
		}
		fields.push_back(t);
		if (!lexer.tryRead(";"))
			throw std::runtime_error("Förväntade \";\"");
	}
	s.tscope->add(std::move(sname), std::make_shared<StructType>(std::move(fields), std::move(fieldnames)));
}