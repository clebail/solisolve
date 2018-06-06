#include "CIndividuFactory.h"
#include "CPlateaux.h"

CIndividu * CIndividuFactory::createIndividu(void) {
	return new CPlateaux();
}

int CIndividuFactory::getTailleGenome(void) {
	return MAX_BILLE-1;
}

int CIndividuFactory::cmp(int score1, int score2) {
	return score2 < score1;
}
