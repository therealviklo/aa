#pragma once
#include <map>
#include <string>
#include <stdexcept>

template <class T>
class Scope
{
private:
	std::map<std::string, T> map;
	Scope* lowerScope;
public:
	Scope(Scope* lowerScope = nullptr) :
		lowerScope(lowerScope) {}
	
	bool contains(const std::string& key) const
	{
		return map.contains(key) || (lowerScope && lowerScope->contains(key));
	}

	bool scopeContains(const std::string& key) const
	{
		return map.contains(key);
	}

	T& operator[](const std::string& key)
	{
		if (map.contains(key))
			return map.at(key);
		if (lowerScope)
			return (*lowerScope)[key];
		throw std::out_of_range("Nyckel \"" + key +  "\" finns inte");
	}

	const T& operator[](const std::string& key) const
	{
		if (map.contains(key))
			return map.at(key);
		if (lowerScope)
			return (*lowerScope)[key];
		throw std::out_of_range("Nyckel \"" + key +  "\" finns inte");
	}

	void add(const std::string& key, T val)
	{
		if (map.contains(key))
			map.erase(key);
		map.emplace(key, std::move(val));
	}

	void foreach(auto f) const
	{
		for (const auto& i : map)
		{
			f(i.second);
		}
		if (lowerScope) lowerScope->foreach(std::move(f));
	}
};