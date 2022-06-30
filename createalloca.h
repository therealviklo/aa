#pragma once
#include "llvm.h"
#include "context.h"

llvm::Value* createAlloca(llvm::Type* type, Context& c);