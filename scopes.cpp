#include "scopes.h"

Scopes::Scopes(Scopes* lowerScope) :
	vscope(std::make_unique<VScope>(lowerScope ? lowerScope->vscope.get() : nullptr)),
	fscope(std::make_unique<FScope>(lowerScope ? lowerScope->fscope.get() : nullptr)),
	tscope(std::make_unique<TScope>(lowerScope ? lowerScope->tscope.get() : nullptr)) {}