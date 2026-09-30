#include <stdio.h>
#include <ncurses.h>
#include "CCoup.h"
#include "CPlateau.h"

CCoup::CCoup(void) {
	this->_isNull = true;
}

CCoup::CCoup(CCoup::ETypeCoup type, int depuis) {
	this->_isNull = false;
	
	this->type = type;
	this->depuis = depuis;
}

const CCoup::ETypeCoup& CCoup::getType(void) {
	return type;
}

const int& CCoup::getDepuis(void) {
	return depuis;
}

void CCoup::print(void) {
    char x = depuis % NB_COLONNE + 'A';
    int y = depuis / NB_COLONNE + 1;
    
    printf("M %c,%d %d\n", x, y, (int)type);
}

// Décalage d'indice entre deux cases voisines dans la direction du coup
int CCoup::getPas(void) {
	switch(type) {
		case etcHaut:
			return -NB_COLONNE;
		case etcDroite:
			return 1;
		case etcBas:
			return NB_COLONNE;
		case etcGauche:
			return -1;
	}

	return 0;
}

// Joue le coup dans le sens du jeu : la bille saute par-dessus sa voisine qui est retirée
void CCoup::joue(unsigned char *plateau) {
	int pas = getPas();

	plateau[depuis] = VIDE;
	plateau[depuis + pas] = VIDE;
	plateau[depuis + pas * 2] = BILLE;
}

bool CCoup::isNull(void) {
	return this->_isNull;
}
