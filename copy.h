#pragma once
#include "type.h"
#include "context.h"
#include "scopes.h"
#include "expression.h"
#include "convert.h"
#include "subscript.h"
#include "name.h"
#include "structtype.h"
#include "structmembynumber.h"

void copy(std::shared_ptr<Expression> from, llvm::Value* to, std::shared_ptr<Type> type, Context& c, Scopes& s);