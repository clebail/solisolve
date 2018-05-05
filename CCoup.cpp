#include <stdio.h>
#include <ncurses.h>
#include "CCoup.h"
#include "CPlateau.h"

CCoup::CCoup(CCoup::ETypeCoup type, int depuis) {
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
    std::string sens = (type == CCoup::etcHaut ? "vers le haut" : (type == CCoup::etcDroite ? "vers la droite" : (type == CCoup::etcBas ? " vers le bas" : "vers la gauche")));
    
    printf("Depuis %c,%d %s\n", x, y, sens.c_str());
}
