#include <iostream>
#include "CPlateaux.h"

void CPlateaux::getCoords(int idx, char& x, int& y) {
	x = idx % NB_COLONNE + 'A';
	y = idx / NB_COLONNE + 1;
}

CGene * CPlateaux::getGene(int idx) {
	return &plateaux[idx];
}

void CPlateaux::init(void) {
	int i;
	
	for(i = 0;i<MAX_BILLE-1;i++) {
		plateaux[i].init(i+1);
	}
	
	calculScore();
}

int CPlateaux::getScore(void) {
	return score;
}

void CPlateaux::calculScore(void) {
	int i;
	int first = false;
	
	coups.clear();
	
	score = 0;
	for(i=0;i<MAX_BILLE-2;i++) {
		CCoup coup;
		if(plateaux[i+1].isNext(&plateaux[i], coup)) {
			if(!first) {
				first = true;
				score += i;
			}
			score += i+1;
			coups.push_back(coup);
		} else {
			first = false;
		}
	}
}

void CPlateaux::mute(int idxGene) {
	plateaux[idxGene].init(plateaux[idxGene].getNbTrou());
}

void CPlateaux::from(CIndividu *i1, CIndividu *i2, int seuil) {
	CPlateaux *source = static_cast<CPlateaux *>(i1);
	int i;
	
	for(i=0;i<MAX_BILLE-1;i++) {
		if(i >= seuil) {
			source = static_cast<CPlateaux *>(i2);
		}
		
		plateaux[i].from(static_cast<CPlateau *>(source->getGene(i)));
	}
}

void CPlateaux::print(void) {
	int depart =  *(plateaux[0].getTrous().begin());
	char x;
	int y;
	std::list<CCoup>::iterator itCoup;
	
	getCoords(depart, x, y);
	std::cout << "D " << x << "," << y;
	
	for(itCoup=coups.begin();itCoup!=coups.end();itCoup++) {
		CCoup coup = *itCoup;
		
		getCoords(coup.getDepuis(), x, y);
		std::cout << " M " << x << "," << y << " " << (int)coup.getType();
	}
}
