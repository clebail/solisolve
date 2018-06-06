#include "CGenetic.h"

int main(void) {
	CGenetic genetic;
	CIndividu *resultat = 0;
	
	resultat = genetic.process();
	
	if(resultat != 0) {
		//todo
	}
    
    return 0;
}
