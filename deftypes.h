#pragma once
#include <memory>
#include "type.h"
#include "scope.h"
#include "integertype.h"
#include "floattype.h"
#include "voidtype.h"
#include "booltype.h"
#include "context.h"

void addDefaultTypes(Context& c, Scope<std::shared_ptr<Type>>& scope);