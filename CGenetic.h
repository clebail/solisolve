#ifndef __CGENETIC__
#define __CGENETIC__

#include "CIndividu.h"

#define TAILLE_POPULATION					150
#define NOMBRE_GENERATION					1000

class CGenetic {
private:
	CIndividu *population[TAILLE_POPULATION];
	
	void initPopulation(void);
	void triPopulation(void);
	bool croisePopuplation(void);
	void croiseIndividus(int i1, int i2, int ir, int seuil);
public:
	CIndividu * process(void);
};

#endif //__CGENETIC__
