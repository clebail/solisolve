#include <stdlib.h>
#include <time.h> 
#include "CGenetic.h"
#include "CIndividuFactory.h"

void CGenetic::initPopulation(void) {
	int i;

	for(i=0;i<TAILLE_POPULATION;i++) {
		population[i] = CIndividuFactory::createIndividu();
		population[i]->init();
	}
}

void CGenetic::triPopulation(void) {
	int i, j;

	for(i=TAILLE_POPULATION-1;i>=1;i--) {
		for(j=0;j<=i-1;j++) {
			if(population[j+1]->getScore() < population[j]->getScore()) {
				CIndividu *tmp = population[j+1];
				
				population[j+1] = population[j];
				population[j] = tmp;
			}
		}
	}
}

bool CGenetic::croisePopuplation(void) {
	int i, ir, max;
	
	i = 1;
	ir = TAILLE_POPULATION - 1;
	max = TAILLE_POPULATION / 2;
	
	while(i < max) {
		int seuil = (rand() % (CIndividuFactory::getTailleGenome() - 2)) + 1;
		
		croiseIndividus(i-1, i, ir, seuil);
		croiseIndividus(i, i-1, ir-1, seuil);
		
		if(population[ir]->getScore() == 0 || population[ir-1]->getScore() == 0) {
			return true;
		}
		
		i+=2;
		ir-=2;
	}
	
	return false;
}

void CGenetic::croiseIndividus(int i1, int i2, int ir, int seuil) {
	population[ir]->from(population[i1], population[i2], seuil);
	
	if(rand() % 10 < 4) {
		population[ir]->mute(rand() % CIndividuFactory::getTailleGenome());
	}
	
	population[ir]->calculScore();
}

CIndividu * CGenetic::process(void) {
	bool fini = false;
	int i = 0;
	
	srand(time(NULL));
		
	initPopulation();
	triPopulation();
	
	do {
		fini = croisePopuplation() | (++i == NOMBRE_GENERATION);
		triPopulation();
	}while(!fini);
	
	return population[0];
}
