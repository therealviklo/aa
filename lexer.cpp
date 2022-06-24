#include "lexer.h"

void Lexer::skipWhitespace()
{
	while (std::isspace(*cur)) cur++;
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

Lexer::Lexer(std::string text) :
	text(std::move(text)),
	cur(this->text.c_str())
{
	tryRead("\xEF\xBB\xBF");
}

char32_t Lexer::getUnicodeChar()
{
	return readUtf8Char(cur);
}

char32_t Lexer::peekUnicodeChar()
{
	const char* tmpcur = cur;
	return readUtf8Char(tmpcur);
}

bool Lexer::tryRead(const char* str)
{
	const char* tmpcur = cur;
	while (*str != '\0')
	{
		if (*str++ != *tmpcur++)
		{
			return false;
		}
	}
	cur = tmpcur;
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
	const char* tmpcur = cur;
	while (*str != '\0')
	{
		if (*str++ != *tmpcur++)
		{
			return false;
		}
	}
	if (isnamechar(*tmpcur))
		return false;
	cur = tmpcur;
	skipWhitespace();
	return true;
}

std::string Lexer::getRegex(const std::regex& re)
{
	std::cmatch m;
	if (!std::regex_search(cur, m, re))
		return "";
	cur = m.suffix().first;
	skipWhitespace();
	return m[0].str();
}