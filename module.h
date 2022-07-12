#pragma once
#include <memory>
#include "llvm.h"
#include "function.h"
#include "variable.h"
#include "scopes.h"
#include "createalloca.h"

void genFunction(const Function& function, Context& c, Scopes& s);
void initModule(const std::string& triple, Context& c);
void genModule(Context& c, Scopes& s);