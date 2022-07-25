#pragma once
#include "type.h"
#include "sameas.h"

struct MutType : public SameAs
{
	MutType(std::shared_ptr<Type> t) :
		SameAs(t) {}
	
	bool isMut() const override { return true; }
	std::string getName() const override;
};

inline std::shared_ptr<Type> makeMut(std::shared_ptr<Type> t)
{
	return t->isMut() ? t : std::make_shared<MutType>(t);
}