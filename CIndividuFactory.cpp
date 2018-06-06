#include "CIndividuFactory.h"
#include "CPlateaux.h"

static int bestScore = -1;

CIndividu * CIndividuFactory::createIndividu(void) {
	return new CPlateaux();
}

int CIndividuFactory::getTailleGenome(void) {
	return MAX_BILLE-1;
}

int CIndividuFactory::cmp(int score1, int score2) {
	return score2 < score1;
}

int CIndividuFactory::getBestScore(void) {
	if(bestScore == -1) {
		int i;
		
		bestScore = 0;
		for(i=1;i<MAX_BILLE;i++) {
			bestScore += i;
		}
	}
	
	return bestScore;
}
