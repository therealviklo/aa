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