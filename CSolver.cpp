#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "CSolver.h"
#include "common.h"

#define MAX_TEST	500

static unsigned char modele[NB_BILLE] = {
	255, 255, 0, 0, 0, 255, 255, 
	255, 0, 0, 0, 0, 0, 255, 
	0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	255, 0, 0, 0, 0, 0, 255,
	255, 255, 0, 0, 0, 255, 255, 
};

void CSolver::init(void) {
	int x, y, idx, nbTest = 0;
	
	srand(time(NULL));	

	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(modele[idx] == VIDE) {
				CPlateau *plateau = new CPlateau(modele, idx);
				
				dout << "Ajout du plateau " << idx << " poids" << plateau->getPoids() << std::endl;
				if(addPlateauIfNotExistst(plateaux, plateau)) {
					if(++nbTest == MAX_TEST) {
                        return;
                    }
				}
			}
		}
	}
	
	std::set<CPlateau *, SPlateauCmp>::iterator it;
	int nb=0;
    x=y=0;
    initscr();
    for(it=plateaux->begin();it!=plateaux->end();it++) {
        (*it)->print(x, y);
        
        if(nb < 10) {
            nb++;
            x+=12;
        } else {
            nb=x=0;
            y+=12;
        }
    }
    refresh();
    getch();
    endwin();
}

bool CSolver::addPlateauIfNotExistst(CPlateaux *plx, CPlateau *plateau) {
	if(!plx->add(plateau)) {
		delete plateau;
		return false;
	}
	
	return true;
}

void CSolver::clearPlateaux(void) {
	
}

CSolver::CSolver(void) {
    plateaux = new CPlateaux();
}

CSolver::~CSolver(void) {
	delete plateaux;
}

int CSolver::getNbPlateaux(void) {
	return plateaux->size();
}

void CSolver::process(void) {
	bool fini = false;
	int nbBille = 1;
	
	init();
	
	while(!fini) {
		printf("Nombre de bille: %2d , nombre de plateau : %10lu\n", nbBille, plateaux->size());
		
		nbBille++;
		fini = nbBille == MAX_BILLE;
		if(!fini) {
			std::set<CPlateau *, SPlateauCmp>::iterator it;
			CPlateaux *newPlateaux = new CPlateaux();
            int nbTest = 0;
			clear();
			
			for(it=plateaux->begin();it!=plateaux->end();it++) {
				CPlateau *pl = *it;
				std::list<CCoup> nextCoups = pl->getNextCoups(rand() % DIFFRENT_COUP);
				std::list<CCoup>::iterator itCoup;
				
				for(itCoup=nextCoups.begin();itCoup!=nextCoups.end();itCoup++) {
                    if(nbTest < MAX_TEST && !(*itCoup).isNull()) {
                        CPlateau *plateau = new CPlateau(pl, *itCoup);
                        
                        if(addPlateauIfNotExistst(newPlateaux, plateau)) {
                            nbTest++;
                        }
                    }
				}
			}
			
			delete plateaux;
            plateaux = newPlateaux;
		}
	}
	
	if(plateaux->size() >= 1) {
        CPlateau *plateau = *(plateaux->begin());
        std::list<CCoup> coups = plateau->getCoups();
        std::list<CCoup>::iterator itCoup;
        
        plateau->printVide();
        
        for(itCoup=coups.begin();itCoup!=coups.end();itCoup++) {
			if(!(*itCoup).isNull()) {
				(*itCoup).print();
			}
        }        
    }
}
