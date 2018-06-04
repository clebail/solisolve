#include "CGenetic.h"

void CGenetic::initPopulation(void) {
	int i;

	for(i=0;i<TAILLE_POPULATION;i++) {
		population[i]->init();
	}
}

void CGenetic::triPopuplation(void) {
	int i, j;

	for(i=TAILLE_POPULATION-1;i>=1;i--) {
		for(j=0;j<=i-1;j++) {
			if(population[j+1]->getScore() < population[j]->getScore()) {
				
			}
		}
	}
}