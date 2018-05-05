#ifndef __SPlateauCmp__
#define __SPlateauCmp__

#include "CPlateau.h"

struct SPlateauCmp {
	bool operator() (CPlateau *p1, CPlateau *p2) {
		return (*p1) < (*p2);
	}
};

#endif //__SPlateauCmp__
