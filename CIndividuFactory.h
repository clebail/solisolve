#ifndef __CINDIVIDUFACTORY__
#define __CINDIVIDUFACTORY__

#include "CIndividu.h"

class CIndividuFactory {
public:
	static CIndividu * createIndividu(void);
	static int getTailleGenome(void);
	static int cmp(int score1, int score2);
	static int getBestScore(void);
};

#endif //__CINDIVIDUFACTORY__
