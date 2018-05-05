#include <ncurses.h>
#include <iostream>
#include "CSolver.h"

#define MAX_TEST        15000

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
	int x, y, idx, nbTest = 0;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(modele[idx] == VIDE) {
				CPlateau *plateau = new CPlateau(modele, idx);
				
				if(addPlateauIfNotExistst(plateaux, plateau)) {
					if(++nbTest == MAX_TEST) {
                        return;
                    }
				}
			}
		}
	}
}

bool CSolver::addPlateauIfNotExistst(std::set<CPlateau *, SPlateauCmp> &plx, CPlateau *plateau) {
	if(plx.find(plateau) != plx.end()) {
		delete plateau;
		return false;
	}
	
	plx.insert(plateau);
	
	return true;
}

void CSolver::clearPlateaux(void) {
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
	clearPlateaux();
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
            int nbTest = 0;
			clear();
			
			for(itPlateau=plateaux.begin();itPlateau!=plateaux.end();itPlateau++) {
				std::list<CCoup> nextCoups = (*itPlateau)->getNextCoups();
				std::list<CCoup>::iterator itCoup;
				
				for(itCoup=nextCoups.begin();itCoup!=nextCoups.end();itCoup++) {
                    if(nbTest < MAX_TEST) {
                        CPlateau *plateau = new CPlateau(*(*itPlateau), *itCoup);
                        
                        if(addPlateauIfNotExistst(newPlateaux, plateau)) {
                            nbTest++;
                        }
                    }
				}
			}
			
			clearPlateaux();
			plateaux.insert(newPlateaux.begin(), newPlateaux.end());
		}
	}
	
	if(plateaux.size() >= 1) {
        std::set<CPlateau *, SPlateauCmp>::iterator itPlateau = plateaux.begin();
        CPlateau *plateau = *itPlateau;
        std::list<CCoup> coups = plateau->getCoups();
        std::list<CCoup>::iterator itCoup;
        
        plateau->printVide();
        
        for(itCoup=coups.begin();itCoup!=coups.end();itCoup++) {
            (*itCoup).print();
        }        
    }
}
