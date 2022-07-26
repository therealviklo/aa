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
			lexer.expect("\"");
			std::string str;
			char32_t ch;
			while ((ch = lexer.getUnicodeChar()) != U'"')
			{
				if (ch == U'\\')
					str += parseEscapedCharacter();
				else
					str += utf32CharToUtf8(ch);
			}
			lexer.expect(";");
			const fs::path newFilename = fs::canonical(fs::path(filename).parent_path() / str);
			if (!visitedFiles->contains(newFilename))
			{
				visitedFiles->emplace(newFilename);
				Parser newp(newFilename, visitedFiles);
				newp.parseFile(s);
			}
		}
		else if (lexer.tryReadName("new"))
		{
			const std::string tname = lexer.expectRegex(nameregex, "typnamn");
			std::shared_ptr<Type> t = getRealType(s.tscope[tname]);
			if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
			{
				lexer.expect("(");
				parseFunctionArgs(
					TypeNamePair(std::make_shared<VoidType>(), Name(st->name)),
					t,
					true,
					s
				);
			}
			else
			{
				lexer.error("Förväntade structtyp");
			}
		}
		else if (lexer.tryRead("~"))
		{
			std::string tname = lexer.expectRegex(nameregex, "typnamn");
			std::shared_ptr<Type> t = getRealType(s.tscope[tname]);
			if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
			{
				lexer.expect("(");
				parseFunctionArgs(
					TypeNamePair{std::make_shared<VoidType>(), Name("~" + st->name)},
					t,
					true,
					s
				);
			}
			else
			{
				lexer.error("Förväntade structtyp");
			}
		}
		else
		{
			std::string token = lexer.getRegex(nameregex);
			if (!s.tscope.contains(token))
				lexer.error("Oväntad token \"" + token + '\"');
			std::shared_ptr<Type> type = parseType(token, s);
			if (lexer.tryRead("->"))
			{
				std::shared_ptr<Type> type2 = parseType(lexer.getRegex(nameregex), s);
				if (!type->isStruct() && !type2->isStruct())
					lexer.error("En av typerna i en konverteringsfunktion måste vara en structtyp");
				lexer.expect("(");
				parseFunctionArgs(
					TypeNamePair{type2, Name(getConvFunName(*type, *type2))},
					type->isStruct() ? type : nullptr,
					false,
					s
				);
			}
			else
			{
				std::string name = lexer.expectRegex(nameregex, "namn");
				std::shared_ptr<Type> methodType;
				if (s.tscope.contains(name))
				{
					std::shared_ptr<Type> t = getRealType(s.tscope[name]);
					if (const StructType* const st = dynamic_cast<const StructType*>(t.get()))
					{
						if (lexer.tryRead("."))
						{
							name = st->name + "$" + lexer.expectRegex(nameregex, "namn");
							methodType = t;
						}
					}
				}
				parseTypeNamePair(TypeNamePair(type, Name(std::move(name))), methodType, false, s);
			}
		}
	}
}

void Parser::parseTypeNamePair(TypeNamePair&& tnp, std::shared_ptr<Type> methodType, bool mut, Scopes& s)
{
	if (lexer.tryRead("("))
	{
		parseFunctionArgs(std::move(tnp), methodType, mut, s);
	}
	else lexer.error("Inte implementerat än ;)");
}

void Parser::parseFunctionArgs(TypeNamePair&& tnp, std::shared_ptr<Type> methodType, bool mut, Scopes& s)
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
			if (!s.tscope.contains(token))
				lexer.error("Oväntad token \"" + token + '\"');
			std::shared_ptr<Type> type = parseType(token, s);
			const std::string name = lexer.expectRegex(nameregex, "namn");
			args.emplace_back(FuncArg{type, name});
		} while (lexer.tryRead(","));
		lexer.expect(")");
	}

	if (lexer.tryReadName("mut"))
		mut = true;
	
	if (lexer.tryRead(";"))
	{
		s.fscope.add(
			tnp.name.name,
			Function{
				tnp.name.name,
				std::move(args),
				varargs,
				tnp.type,
				methodType,
				mut,
				nullptr
			}
		);
	}
	else
	{
		Scopes as(&s);
		std::shared_ptr<Statement> body = parseStatement(as);
		s.fscope.add(
			tnp.name.name,
			Function{
				tnp.name.name,
				std::move(args),
				varargs,
				tnp.type,
				methodType,
				mut,
				std::move(body)
			}
		);
	}
}

std::shared_ptr<Statement> Parser::parseStatement(Scopes& s)
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
		std::shared_ptr<Statement> expr = parseExpression(0, s);
		lexer.expect(";");
		return expr;
	}
}

std::shared_ptr<Statement> Parser::parseCompoundStatement(Scopes& s)
{
	std::vector<std::shared_ptr<Statement>> stmts;
	while (!lexer.tryRead("}"))
	{
		stmts.push_back(parseStatement(s));
	}
	return std::make_shared<Block>(std::move(stmts));
}

std::shared_ptr<Expression> Parser::parseExpression(int lvl, Scopes& s)
{
#define mapPreUnOp(str, Op) if (lexer.tryRead(str)) \
	{ \
		std::shared_ptr<Expression> expr = parseExpression(prelevel(str), s); \
		return parseExpressionRight( \
			lvl, \
			std::make_shared<Op>(std::move(expr)), \
			s \
		); \
	}
	mapPreUnOp("&", AddrOf);
	mapPreUnOp("-", Neg);
	mapPreUnOp("!", Not);
	if (lexer.tryRead("+"))
	{
		std::shared_ptr<Expression> expr = parseExpression(prelevel("+"), s);
		return parseExpressionRight(
			lvl,
			std::move(expr),
			s
		);
	}
	if (lexer.tryRead("("))
	{
		std::shared_ptr<Expression> expr = parseExpression(0, s);
		lexer.expect(")");
		return parseExpressionRight(
			lvl,
			std::move(expr),
			s
		);
	}
	if (lexer.tryRead("[["))
	{
		std::string tname = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> type = parseType(tname, s);
		lexer.expect("]]");
		std::shared_ptr<Expression> expr = parseExpression(prelevel("[["), s);
		return parseExpressionRight(
			lvl,
			std::make_shared<BitCast>(std::move(expr), type),
			s
		);
	}
	if (lexer.tryRead("["))
	{
		std::string tname = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> type = parseType(tname, s);
		lexer.expect("]");
		std::shared_ptr<Expression> expr = parseExpression(prelevel("["), s);
		return parseExpressionRight(
			lvl,
			std::make_shared<Convert>(std::move(expr), type),
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
			std::make_shared<StringLit>(
				std::move(str)
			),
			s
		);
	}
	if (lexer.tryRead("\""))
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
			std::make_shared<CreateStruct>(
				s.tscope["String"],
				std::vector<std::shared_ptr<Expression>>({
					std::make_shared<StringLit>(
						str
					),
					std::make_shared<Name>(
						std::to_string(str.size()) + "u"
					)
				})
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
			lexer.error("Teckenkonstant måste bestå av ett enda enwordigt tecken.");
		std::stringstream ss;
		ss << static_cast<std::uint32_t>(str[0]) << "u8";
		return parseExpressionRight(
			lvl,
			std::make_shared<Name>(
				ss.str()
			),
			s
		);
	}
	if (lexer.tryReadName("sizeof"))
	{
		lexer.expect("(");
		std::string name = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> t = parseType(name, s);
		lexer.expect(")");
		return parseExpressionRight(
			lvl,
			std::make_shared<SizeOf>(t),
			s
		);
	}
	if (lexer.tryReadName("new"))
	{
		const std::string tname = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> type = s.tscope[tname];
		lexer.expect("(");
		auto args = parseFuncCallArgs(s);
		return parseExpressionRight(
			lvl,
			std::make_shared<CreateStruct>(type, args),
			s
		);
	}

	std::string operand = lexer.expectRegex(nameregex, "värde");
	if (s.tscope.contains(operand))
	{
		std::shared_ptr<Type> type = parseType(operand, s);
		if (lexer.tryRead("."))
		{
			std::shared_ptr<Type> t2 = getRealType(type);
			if (const StructType* const st = dynamic_cast<const StructType*>(t2.get()))
			{
				const std::string fname = lexer.expectRegex(nameregex, "funktionsnamn");
				return parseExpressionRight(
					lvl,
					std::make_shared<Name>(st->name + "$" + fname),
					s
				);
			}
			else
			{
				lexer.error("Förväntade att typen är en struct");
			}
		}
		else
		{
			std::string name = lexer.expectRegex(nameregex, "namn");
			return parseExpressionRight(
				lvl,
				std::make_shared<TypeNamePair>(
					type,
					Name(std::move(name))
				),
				s
			);
		}
	}
	else
	{
		return parseExpressionRight(lvl, std::make_shared<Name>(std::move(operand)), s);
	}
}

std::shared_ptr<Expression> Parser::parseExpressionRight(int lvl, std::shared_ptr<Expression> left, Scopes& s)
{
#define mapBinOp(str, Op) if (checkLevel(level(str), lvl) && lexer.tryRead(str)) \
		{ \
			return parseExpressionRight( \
				lvl, \
				std::make_shared<Op>( \
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
				std::make_shared<Op>( \
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
		if (lexer.tryReadName("new"))
		{
			const std::string tname = lexer.expectRegex(nameregex, "typnamn");
			std::shared_ptr<Type> type = s.tscope[tname];
			lexer.expect("(");
			auto args = parseFuncCallArgs(s);
			return parseExpressionRight(
				lvl,
				std::make_shared<InPlaceConstruct>(std::move(left), type, std::move(args)),
				s
			);
		}
		else if (lexer.tryRead("~"))
		{
			const std::string tname = lexer.expectRegex(nameregex, "typnamn");
			std::shared_ptr<Type> type = getRealType(s.tscope[tname]);
			if (const StructType* const st = dynamic_cast<const StructType*>(type.get()))
			{
				lexer.expect("(");
				lexer.expect(")");
				return parseExpressionRight(
					lvl,
					std::make_shared<FuncCall>(
						std::make_shared<Name>("~" + st->name),
						std::vector<std::shared_ptr<Expression>>({
							std::make_shared<AddrOf>(left)
						})
					),
					s
				);
			}
			else
			{
				lexer.error("Förväntade att typen är en struct");
			}
		}
		else
		{
			std::string fieldname = lexer.expectRegex(nameregex, "namn");
			return parseExpressionRight(
				lvl,
				std::make_shared<DotOp>(
					std::move(left),
					std::move(fieldname)
				),
				s
			);
		}
	}
	
	if (checkLevel(level("("), lvl) && lexer.tryRead("("))
	{
		auto args = parseFuncCallArgs(s);
		return parseExpressionRight(
			lvl,
			std::make_shared<FuncCall>(
				std::move(left),
				std::move(args)
			),
			s
		);
	}
	if (checkLevel(level("["), lvl) && lexer.tryRead("["))
	{
		std::shared_ptr<Expression> expr = parseExpression(0, s);
		lexer.expect("]");
		return parseExpressionRight(
			lvl,
			std::make_shared<Subscript>(
				std::move(left),
				std::move(expr)
			),
			s
		);
	}

	return left;
}

std::vector<std::shared_ptr<Expression>> Parser::parseFuncCallArgs(Scopes& s)
{
	std::vector<std::shared_ptr<Expression>> args;
	if (!lexer.tryRead(")"))
	{
		do
		{
			args.push_back(parseExpression(0, s));
		} while (lexer.tryRead(","));
		lexer.expect(")");
	}
	return args;
}

std::shared_ptr<Statement> Parser::parseReturnStatement(Scopes& s)
{
	if (lexer.tryRead(";"))
		return std::make_shared<Return>(nullptr);
	std::shared_ptr<Expression> expr = parseExpression(0, s);
	lexer.expect(";");
	return std::make_shared<Return>(std::move(expr));
}

std::shared_ptr<Statement> Parser::parseIfStatement(Scopes& s)
{
	Scopes as(&s);
	lexer.expect("(");
	std::shared_ptr<Expression> cond = parseExpression(0, as);
	lexer.expect(")");
	Scopes aas(&as);
	std::shared_ptr<Statement> thenStmt = parseStatement(aas);
	if (lexer.tryReadName("else"))
	{
		Scopes aas2(&as);
		std::shared_ptr<Statement> elseStmt = parseStatement(aas2);
		return std::make_shared<IfElse>(std::move(cond), std::move(thenStmt), std::move(elseStmt));
	}
	else
	{
		return std::make_shared<If>(std::move(cond), std::move(thenStmt));
	}
}

std::shared_ptr<Statement> Parser::parseWhileStatement(Scopes& s)
{
	Scopes as(&s);
	lexer.expect("(");
	std::shared_ptr<Expression> cond = parseExpression(0, as);
	lexer.expect(")");
	Scopes aas(&as);
	std::shared_ptr<Statement> stmt = parseStatement(aas);
	return std::make_shared<While>(std::move(cond), std::move(stmt));
}

std::shared_ptr<Type> Parser::parseType(const std::string& name, Scopes& s)
{
	std::shared_ptr<Type> type = s.tscope[name];
	while (true)
	{
		if (lexer.tryReadName("mut"))
		{
			if (type->isRef())
				lexer.error("Referenser får inte göras mut");
			if (type->isMut())
				lexer.error("Dubbel mut är inte tillåtet");
			type = std::make_shared<MutType>(type);
		}
		else if (lexer.tryRead("*"))
		{
			if (type->isVoid())
				type = std::make_shared<PointerType>(
					type->isMut()
					? makeMut(s.tscope["u8"])
					: s.tscope["u8"]
				);
			else
				type = std::make_shared<PointerType>(type);
		}
		else if (lexer.tryRead("["))
		{
			const std::string numstr = lexer.expectRegex(numregex, "tal");
			lexer.expect("]");
			const unsigned long long num = std::stoull(numstr);
			type = std::make_shared<ArrayType>(type, num);
		}
		else if (lexer.tryRead("&"))
		{
			if (type->isRef())
				lexer.error("Dubbla referenstyper är inte tillåtna");
			type = std::make_shared<RefType>(type);
		}
		else if (lexer.tryRead("<-"))
		{
			lexer.expect("(");
			std::vector<std::shared_ptr<Type>> argTypes;
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
					if (!s.tscope.contains(token))
						lexer.error("Oväntad token \"" + token + '\"');
					std::shared_ptr<Type> type = parseType(token, s);
					argTypes.emplace_back(type);
				} while (lexer.tryRead(","));
				lexer.expect(")");
			}
			type = std::make_shared<FunctionType>(type, std::move(argTypes), varargs);
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
	lexer.error("Okänd escapesekvens");
}

void Parser::parseTypeDecl(Scopes& s)
{
	std::string name = lexer.expectRegex(nameregex, "namn");
	if (lexer.tryRead(";"))
	{
		s.tscope.add(name, std::make_shared<FutureType>(s.tscope, std::move(name)));
	}
	else if (lexer.tryRead("="))
	{
		std::string oname = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> t = parseType(oname, s);
		lexer.expect(";");
		s.tscope.add(name, t);
	}
	else
	{
		bool packed = false;
		if (lexer.tryReadName("packed"))
			packed = true;
		lexer.expect("{");
		s.tscope.add(name, std::make_shared<FutureType>(s.tscope, name));
		parseStruct(std::move(name), packed, s);
	}
}

void Parser::parseStruct(std::string sname, bool packed, Scopes& s)
{
	std::vector<std::shared_ptr<Type>> fields;
	std::map<std::string, size_t> fieldnames;
	while (!lexer.tryRead("}"))
	{
		std::string tname = lexer.expectRegex(nameregex, "typnamn");
		std::shared_ptr<Type> t = parseType(tname, s);
		std::string name = lexer.getRegex(nameregex);
		if (!name.empty())
		{
			fieldnames.emplace(name, fields.size());
		}
		fields.push_back(t);
		lexer.expect(";");
	}
	s.tscope.add(sname, std::make_shared<StructType>(std::move(fields), std::move(fieldnames), sname, packed));
}