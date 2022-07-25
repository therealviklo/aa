#include "name.h"

const std::regex intregex("(^[0-9]+|^0x[0-9a-fA-F]+|^0o[0-7]+|^0b[01]+)(?=[iu][0-9]*$)");
const std::regex intregex2("[0-9a-fA-F]*$");
const std::regex hexregex("^0x");
const std::regex octregex("^0o");
const std::regex binregex("^0b");
const std::regex uregex("u[0-9]*$");
// const std::regex iregex("i[0-9]*$");
const std::regex bitregex("[0-9]+$");
const std::regex f16regex("^[0-9]+\\.?[0-9]*|\\.[0-9]+(?=f16$)");
const std::regex f32regex("^[0-9]+\\.?[0-9]*|\\.[0-9]+(?=f32$)");
const std::regex f64regex("^[0-9]+\\.?[0-9]*|\\.[0-9]+(?=(f64)?$)");
const std::regex inff16regex("inff16|infinityf16", std::regex::icase);
const std::regex inff32regex("inff32|infinityf32", std::regex::icase);
const std::regex inff64regex("inf(f64)?|infinity(f64)?", std::regex::icase);
const std::regex nanf16regex("nanf16", std::regex::icase);
const std::regex nanf32regex("nanf32", std::regex::icase);
const std::regex nanf64regex("nan(f64)?", std::regex::icase);

namespace
{
	std::shared_ptr<Type> getSmallestPossibleIntType(unsigned long long i, Scopes& s)
	{
		if (i <= 0xFFull)
			return s.tscope["u8"];
		if (i <= 0xFFFFull)
			return s.tscope["u16"];
		if (i <= 0xFFFFFFFFull)
			return s.tscope["u32"];
		return s.tscope["u64"];
	}

	std::shared_ptr<Type> getSmallestPossibleSignedIntType(unsigned long long i, Scopes& s)
	{
		if (i <= 0x7Full)
			return s.tscope["i8"];
		if (i <= 0x7FFFull)
			return s.tscope["i16"];
		if (i <= 0x7FFFFFFFull)
			return s.tscope["i32"];
		return s.tscope["i64"];
	}
}

// llvm::Value* Name::getRefValue(Context& /*c*/, Scopes& s) const
// {
// 	if (!s.vscope.contains(name))
// 		throw std::runtime_error("Okänd variabel");
// 	if (!s.vscope[name].ref)
// 		throw std::runtime_error("Inte en referens");
// 	return s.vscope[name].var;
// }

llvm::Value* Name::get(Context& c, Scopes& s) const
{
	if (s.vscope.contains(name))
	{
		if (s.vscope[name].type->isRef())
			return s.vscope[name].getValue(c);
		return s.vscope[name].var;
	}
	llvm::Type* const t = getTypeC(c, s)->getType(*c.c);
	if (name == "true")
		return llvm::ConstantInt::get(t, 1);
	if (name == "false")
		return llvm::ConstantInt::get(t, 0);
	if (name == "null")
		return llvm::ConstantPointerNull::get(llvm::PointerType::get(*c.c, 0));
	if (s.fscope.contains(name))
		return s.fscope[name].getFunction(c);
	std::smatch m;
	std::string i;
	if (std::regex_search(name, m, intregex) && std::regex_search(i = m[0].str(), m, intregex2))
	{
		int base = 10;
		if (std::regex_search(name, hexregex))
			base = 16;
		if (std::regex_search(name, octregex))
			base = 8;
		if (std::regex_search(name, binregex))
			base = 2;
		if (std::regex_search(name, uregex))
		{
			return llvm::ConstantInt::get(t, std::stoull(m[0].str(), nullptr, base));
		}
		else
		{
			return llvm::ConstantInt::getSigned(t, std::stoull(m[0].str(), nullptr, base));
		}
	}
	if (std::regex_search(name, m, f16regex))
		return llvm::ConstantFP::get(t, std::stod(m[0].str()));
	if (std::regex_search(name, m, f32regex))
		return llvm::ConstantFP::get(t, std::stod(m[0].str()));
	if (std::regex_search(name, m, f64regex))
		return llvm::ConstantFP::get(t, std::stod(m[0].str()));
	if (std::regex_match(name, inff16regex))
		return llvm::ConstantFP::get(t, INFINITY);
	if (std::regex_match(name, inff32regex))
		return llvm::ConstantFP::get(t, INFINITY);
	if (std::regex_match(name, inff64regex))
		return llvm::ConstantFP::get(t, INFINITY);
	if (std::regex_match(name, nanf16regex))
		return llvm::ConstantFP::get(t, NAN);
	if (std::regex_match(name, nanf32regex))
		return llvm::ConstantFP::get(t, NAN);
	if (std::regex_match(name, nanf64regex))
		return llvm::ConstantFP::get(t, NAN);
	throw std::runtime_error("Okänd variabel");
}

std::shared_ptr<Type> Name::getType(Context& /*c*/, Scopes& s) const
{
	if (name == "true" || name == "false")
		return s.tscope["bool"];
	if (name == "null")
		return std::make_shared<PointerType>(s.tscope["void"]);
	if (s.fscope.contains(name))
		return s.fscope[name].getFunctionType();
	std::smatch m;
	std::string i;
	if (std::regex_search(name, m, intregex) && std::regex_search(i = m[0].str(), m, intregex2))
	{
		int base = 10;
		if (std::regex_search(name, hexregex))
			base = 16;
		if (std::regex_search(name, octregex))
			base = 8;
		if (std::regex_search(name, binregex))
			base = 2;
		std::smatch bm;
		if (std::regex_search(name, uregex))
		{
			if (std::regex_search(name, bm, bitregex))
			{
				return s.tscope["u" + bm[0].str()];
			}
			else
			{
				return getSmallestPossibleIntType(std::stoull(m[0].str(), nullptr, base), s);
			}
		}
		else
		{
			if (std::regex_search(name, bm, bitregex))
			{
				return s.tscope["i" + bm[0].str()];
			}
			else
			{
				return getSmallestPossibleSignedIntType(std::stoull(m[0].str(), nullptr, base), s);
			}
		}
	}
	if (std::regex_search(name, f16regex))
		return s.tscope["f16"];
	if (std::regex_search(name, f32regex))
		return s.tscope["f32"];
	if (std::regex_search(name, f64regex))
		return s.tscope["f64"];
	if (std::regex_search(name, inff16regex))
		return s.tscope["f16"];
	if (std::regex_search(name, inff32regex))
		return s.tscope["f32"];
	if (std::regex_search(name, inff64regex))
		return s.tscope["f64"];
	if (std::regex_search(name, nanf16regex))
		return s.tscope["f16"];
	if (std::regex_search(name, nanf32regex))
		return s.tscope["f32"];
	if (std::regex_search(name, nanf64regex))
		return s.tscope["f64"];
	return s.vscope[name].ref ? makeRef(s.vscope[name].type) : s.vscope[name].type;
}

llvm::Value* Name::createCall(std::vector<llvm::Value*> args, Context& c, Scopes& s) const
{
	if (s.fscope.contains(name))
		return c.builder->CreateCall(s.fscope[name].getFunction(c), args);
	return Expression::createCall(std::move(args), c, s);
}