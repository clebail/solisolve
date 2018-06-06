#include <stdlib.h>
#include <string.h>
#include "CPlateau.h"

static unsigned char modele[NB_BILLE] = {
	255, 255, 1, 1, 1, 255, 255, 
	255, 1, 1, 1, 1, 1, 255, 
	1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 
	255, 1, 1, 1, 1, 1, 255,
	255, 255, 1, 1, 1, 255, 255, 
};

void CPlateau::init(int nbTrou) {
	int nb = 0;
	
	memcpy(plateau, modele, sizeof(unsigned char) * NB_BILLE);
	
	while(nb != nbTrou) {
		int idx = rand() % NB_BILLE;
		
		if(plateau[idx] == BILLE) {
			plateau[idx] = VIDE;
			nb++;
		}
	}
	
	this->nbTrou = nbTrou;
}

int CPlateau::getNbTrou(void) {
	return nbTrou;
}

void CPlateau::from(CPlateau * other) {
	memcpy(plateau, other->plateau, sizeof(unsigned char) * NB_BILLE);
}
