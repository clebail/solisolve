#include "CPlateaux.h"

CGene * CPlateaux::getGene(int idx) {
	return &plateaux[idx];
}

void CPlateaux::init(void) {
	int i;
	
	for(i = 0;i<NB_BILLE-1;i++) {
		plateaux[i].init(i+1);
	}
	
	calculScore();
}

int CPlateaux::getScore(void) {
	return score;
}

void CPlateaux::calculScore(void) {
	score = 0;
}

void CPlateaux::mute(int idxGene) {
	plateaux[idxGene].init(plateaux[idxGene].getNbTrou());
}

void CPlateaux::from(CIndividu *i1, CIndividu *i2, int seuil) {
	CPlateaux *source = static_cast<CPlateaux *>(i1);
	int i;
	
	for(i=0;i<NB_BILLE-1;i++) {
		if(i >= seuil) {
			source = static_cast<CPlateaux *>(i2);
		}
		
		plateaux[i].from(static_cast<CPlateau *>(source->getGene(i)));
	}
}
