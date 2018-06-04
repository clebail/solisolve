#ifndef __CGENETIC__
#define __CGENETIC__

#include "CIndividu.h"

#define TAILLE_POPULATION					150

class CGenetic {
private:
	CIndividu *population[TAILLE_POPULATION];
	
	void initPopulation(void);
	void triPopuplation(void);
public:
};

#endif //__CGENETIC__