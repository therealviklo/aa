#pragma once
#include "llvm.h"
#include "context.h"

llvm::AllocaInst* createAlloca(llvm::Type* type, Context& c);