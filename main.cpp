#if DLTA_BUILD
#define DLTA_EDITOR 0
#else
#define DLTA_EDITOR 1
#endif

// run 2d sample
//#define _2D

#ifdef _2D
#include "Game/test2D.h"
#else
#include "Game/test3D.h"
#endif

void main() {
#ifdef _2D
	test2D();
#else
	test3D();
#endif
}
