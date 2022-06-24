#pragma once
#include "type.h"
#include "sameas.h"

class RefType : public SameAs
{
public:
	RefType(std::shared_ptr<Type> t) :
		SameAs(t) {}
	
	bool isRef() const override { return true; }
};