#if DLTA_BUILD
#define DLTA_EDITOR 0
#else
#define DLTA_EDITOR 1
#endif

// run 2d sample
//#define _2D
//#define _3D
#define _PONG

#ifdef _2D
#include "Game/test2D.h"
#endif
#ifdef _3D
#include "Game/test3D.h"
#endif
#ifdef _PONG
#include "Game/Pong/Pong.h"
#endif

int main() {
	return game();
}
