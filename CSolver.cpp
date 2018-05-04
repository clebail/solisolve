#include <ncurses.h>
#include <iostream>
#include "CSolver.h"

static unsigned char modele[NB_BILLE] = {
	255, 255, 255, 0, 0, 0, 255, 255, 255,
	255, 255, 0, 0, 0, 0, 0, 255, 255, 
	255, 0, 0, 0, 0, 0, 0, 0, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	255, 0, 0, 0, 0, 0, 0, 0, 255,
	255, 255, 0, 0, 0, 0, 0, 255, 255, 
	255, 255, 255, 0, 0, 0, 255, 255, 255,
};

void CSolver::init(void) {
	int x, y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(modele[idx] == VIDE) {
				CPlateau *plateau = new CPlateau(modele, idx);
				
				addPlateauIfNotExistst(plateaux, plateau);
			}
		}
	}
}

bool CSolver::addPlateauIfNotExistst(std::set<CPlateau *, SPlateauCmp> &plateaux, CPlateau *plateau) {
	if(plateaux.find(plateau) != plateaux.end()) {
		delete plateau;
		return false;
	}
	
	plateaux.insert(plateau);
	
	return true;
}

void CSolver::clear(void) {
	std::set<CPlateau *, SPlateauCmp>::iterator i = plateaux.begin();
	
	while(i != plateaux.end()) {
		CPlateau *p = *i;
		
		delete p;
		
		i++;
	}
	
	plateaux.clear();
}

CSolver::CSolver(void) {
}

CSolver::~CSolver(void) {
}

int CSolver::getNbPlateaux(void) {
	return plateaux.size();
}

void CSolver::process(void) {
	bool fini = false;
	int nbBille = 1;
	
	init();
	
	while(!fini) {
		printf("Nombre de bille: %d , nombre de plateau : %lu\n", nbBille, plateaux.size());
		
		nbBille++;
		fini = nbBille == MAX_BILLE;
		if(!fini) {
			std::set<CPlateau *, SPlateauCmp>::iterator itPlateau;
			std::set<CPlateau *, SPlateauCmp> newPlateaux;
			
			for(itPlateau=plateaux.begin();itPlateau!=plateaux.end();itPlateau++) {
				std::list<CCoup> nextCoups = (*itPlateau)->getNextCoups();
				std::list<CCoup>::iterator itCoup;
				
				printf("%lu coups possible\n", nextCoups.size());
				
				for(itCoup=nextCoups.begin();itCoup!=nextCoups.end();itCoup++) {
					CPlateau *plateau = new CPlateau(*(*itPlateau), *itCoup);
					
					addPlateauIfNotExistst(newPlateaux, plateau);
				}
			}
			
			clear();
			plateaux.insert(newPlateaux.begin(), newPlateaux.end());
		}
	}
}
