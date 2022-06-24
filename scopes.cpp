#include "scopes.h"

Scopes::Scopes(Scopes* lowerScope) :
	vscope(lowerScope ? &lowerScope->vscope : nullptr),
	fscope(lowerScope ? &lowerScope->fscope : nullptr),
	tscope(lowerScope ? &lowerScope->tscope : nullptr) {}