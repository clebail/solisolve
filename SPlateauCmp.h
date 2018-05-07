#ifndef __SPlateauCmp__
#define __SPlateauCmp__

#include "CPlateau.h"

struct SPlateauCmp {
	bool operator() (CPlateau *p) {
		return (*toFind) == (*p);
	}
	
	CPlateau *toFind;
};

#endif //__SPlateauCmp__
