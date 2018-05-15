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

bool CCoup::isNull(void) {
	return this->_isNull;
}
