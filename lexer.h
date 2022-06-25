#pragma once
#include <string>
#include <cctype>
#include <cwctype>
#include <stdexcept>
#include <regex>

extern const std::regex nameregex;
extern const std::regex numregex;

std::string utf32CharToUtf8(char32_t c);

struct LineColCur
{
	const char* cur;
	size_t line;
	size_t col;

	char32_t getUnicodeChar();
	char32_t peekUnicodeChar() const;
};

class Lexer
{
private:
	std::string text;
	LineColCur cur;

	void skipWhitespace();
public:
	Lexer(std::string text);

	char32_t getUnicodeChar();
	char32_t peekUnicodeChar();

	bool atEnd() { return *cur.cur == '\0'; }
	bool tryRead(const char* str);
	bool tryReadName(const char* str);
	std::string getRegex(const std::regex& re);

	[[noreturn]] void error(const std::string& msg);
	void expect(const char* str);
	void expectName(const char* str);
};