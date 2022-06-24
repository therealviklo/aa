#pragma once
#include <memory>
#include <string>

struct Type;

struct FuncArg
{
	std::shared_ptr<Type> type;
	std::string name;
};