#include "lexer.h"

void Lexer::skipWhitespace()
{
	auto skipComment = [&]() -> bool {
		if (tryRead("//", false))
		{
			while (!atEnd() && getUnicodeChar() != U'\n');
			return true;
		}
		else if (tryRead("/*", false))
		{
			while (!atEnd() && !tryRead("*/", false))
				getUnicodeChar();
			return true;
		}
		return false;
	};
	do
	{
		while (std::isspace(peekUnicodeChar()))
			getUnicodeChar();
	} while (skipComment());
}

const std::regex nameregex(
	"^[0-9]+\\.?[0-9]*(f16|f32|(f64)?)(?=$|[^a-zA-Z0-9_])"
	"|^\\.[0-9]+(f16|f32|(f64)?)(?=$|[^a-zA-Z0-9_])"
	"|^[a-zA-Z0-9_]+"
);
const std::regex numregex(
	"^[0-9]+"
);

namespace
{
	char32_t readUtf8Char(const char*& cur)
	{
		if (static_cast<unsigned char>(*cur) < 0b1000'0000)
			return *cur++;
		char32_t ret = 0;
		int numConts = 0;
		if (static_cast<unsigned char>(*cur) >= 0b1111'1000)
		{
			cur++;
			return U'\uFFFD';
		}
		if (static_cast<unsigned char>(*cur) >= 0b1111'0000)
		{
			ret = *cur++ & 0b0000'0111;
			numConts = 3;
		}
		else if (static_cast<unsigned char>(*cur) >= 0b1110'0000)
		{
			ret = *cur++ & 0b0000'1111;
			numConts = 2;
		}
		else if (static_cast<unsigned char>(*cur) >= 0b110'0000)
		{
			ret = *cur++ & 0b0001'1111;
			numConts = 1;
		}
		else
		{
			cur++;
			return U'\uFFFD';
		}
		while (static_cast<unsigned char>(*cur) >= 0b1000'0000 && static_cast<unsigned char>(*cur) < 0b1100'0000)
		{
			if (numConts-- == 0)
				return U'\uFFFD';
			ret <<= 6;
			ret |= *cur++ & 0b0011'1111;
		}
		if (numConts != 0 || ret >= 0x110000)
			return U'\uFFFD';
		return ret;
	}
}

std::string utf32CharToUtf8(char32_t c)
{
	auto g = [&c](int offset, int num) -> char32_t {
		return (c >> offset) & ~(~U'\0' << num);
	};
	auto gbytes = [&g](int num) -> std::string {
		std::string ret;
		ret += g(6 * (num - 1), 7 - num) | (0b1111'0000 & (0b1111'0000 << (4 - num)));
		for (int i = 0; i < num - 1; i++)
		{
			ret += g(6 * (num - 2 - i), 6) | 0b1000'0000;
		}
		return ret;
	};
	if (c >= 0x110000)
		return "\uFFFD";
	else if (c >= 0x10000)
		return gbytes(4);
	else if (c >= 0x800)
		return gbytes(3);
	else if (c >= 0x80)
		return gbytes(2);
	else
		return std::string(1, static_cast<char>(c));
}

char32_t LineColCur::getUnicodeChar()
{
	const char32_t c = readUtf8Char(cur);
	if (!c)
		throw std::runtime_error("Oväntat filslut");
	if (c == U'\n')
	{
		line++;
		col = 1;
	}
	else
	{
		col++;
	}
	return c;
}

char32_t LineColCur::peekUnicodeChar() const
{
	const char* tmpcur = cur;
	return readUtf8Char(tmpcur);
}

Lexer::Lexer(std::string text) :
	text(std::move(text)),
	cur{this->text.c_str(), 1, 1}
{
	tryRead("\xEF\xBB\xBF");
	skipWhitespace();
}

char32_t Lexer::getUnicodeChar()
{
	return cur.getUnicodeChar();
}

char32_t Lexer::peekUnicodeChar()
{
	return cur.peekUnicodeChar();
}

bool Lexer::tryRead(const char* str, bool doSkipWhitespace)
{
	LineColCur tmpcur = cur;
	while (*str != '\0')
	{
		if (!*tmpcur.cur || readUtf8Char(str) != tmpcur.getUnicodeChar())
		{
			return false;
		}
	}
	cur = tmpcur;
	if (doSkipWhitespace)
		skipWhitespace();
	return true;
}

namespace
{
	bool isnamechar(char32_t c)
	{
		return c < 0x10000 && (std::iswalnum(static_cast<wchar_t>(c & 0xFFFF)) || c == U'_');
	}
}

bool Lexer::tryReadName(const char* str)
{
	LineColCur tmpcur = cur;
	while (*str != '\0')
	{
		if (!*tmpcur.cur || readUtf8Char(str) != tmpcur.getUnicodeChar())
		{
			return false;
		}
	}
	if (isnamechar(tmpcur.peekUnicodeChar()))
		return false;
	cur = tmpcur;
	skipWhitespace();
	return true;
}

std::string Lexer::getRegex(const std::regex& re)
{
	std::cmatch m;
	if (!std::regex_search(cur.cur, m, re))
		return "";
	const char* mcur = m[0].first;
	while (mcur < m[0].second)
	{
		readUtf8Char(mcur);
		cur.getUnicodeChar();
	}
	skipWhitespace();
	return m[0].str();
}

void Lexer::error(const std::string& msg)
{
	std::stringstream ss;
	ss << msg << " (rad " << cur.line << ", kolumn " << cur.col << ")";
	throw std::runtime_error(ss.str());
}

void Lexer::expect(const std::string& str)
{
	if (!tryRead(str.c_str()))
	{
		if (str == "\"")
			error("Förväntade \"");
		else
			error("Förväntade \"" + str + "\"");
	}
}

void Lexer::expectName(const std::string& str)
{
	if (!tryReadName(str.c_str()))
		error("Förväntade \"" + str + "\"");
}

std::string Lexer::expectRegex(const std::regex& re, const std::string& name)
{
	const std::string str = getRegex(re);
	if (str.empty())
		error("Förväntade " + name);
	return str;
}