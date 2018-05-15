#ifndef __SPlateauCmp__
#define __SPlateauCmp__

#include "CPlateau.h"

struct SPlateauCmp {
	bool operator() (CPlateau *p1, CPlateau *p2) {
		return p1->getPoids() < p2->getPoids();
	}
};

#endif //__SPlateauCmp__
