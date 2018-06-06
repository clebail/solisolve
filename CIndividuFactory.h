#ifndef __CINDIVIDUFACTORY__
#define __CINDIVIDUFACTORY__

#include "CIndividu.h"

class CIndividuFactory {
public:
	static CIndividu * createIndividu(void);
	static int getTailleGenome(void);
};

#endif //__CINDIVIDUFACTORY__
