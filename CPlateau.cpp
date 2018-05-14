#include <string.h>
#include <ncurses.h>
#include "CPlateau.h"
#include "common.h"

typedef CCoup (*ptrFctCoup)(unsigned char *, int, int, int);

static unsigned modele[NB_BILLE] = {
	0,  0,  1,   4,  1,  0, 0,
	0,  8,  2,  16,  2,  8, 0,   
	1,  2, 32,  64, 32,  2, 1,
	4, 16, 64, 128, 64, 16, 4,
	1,  2, 32,  64, 32,  2, 1,
	0,  8,  2,  16,  2,  8, 0,  
	0,  0,  1,   4,  1,  0, 0,
};

static CCoup haut(unsigned char *plateau, int x, int y, int idx);
static CCoup droite(unsigned char *plateau, int x, int y, int idx);
static CCoup bas(unsigned char *plateau, int x, int y, int idx);
static CCoup gauche(unsigned char *plateau, int x, int y, int idx);

static ptrFctCoup fctCoup[DIFFRENT_COUP] { haut, droite, bas, gauche };

void CPlateau::mirror(unsigned char *pl) {
	int x, y, idx, idxOther;
    
    if(pl == 0) {
        pl = plateau;
    }
	
	for(y=idx=0;y<NB_LIGNE/2;y++) {
		idxOther = NB_BILLE - (y + 1) * NB_COLONNE;
		for(x=0;x<NB_COLONNE;x++,idx++,idxOther++) {
			swap(&pl[idx], &pl[idxOther]);
		}
	}
}

void CPlateau::rotate(unsigned char *pl) {
	unsigned char newPlateau[NB_BILLE];
	int x, y, idx, newIdx;
    
    if(pl == 0) {
        pl = plateau;
    }
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			newIdx = (NB_LIGNE - x - 1) * NB_COLONNE + y;
			newPlateau[newIdx] = pl[idx];
		}
	}
	
	memcpy(pl, newPlateau, NB_BILLE * sizeof(unsigned char));
}

void CPlateau::swap(unsigned char *c1, unsigned char *c2) {
	unsigned char t = *c1;
	*c1 = *c2;
	*c2 = t;
}

void CPlateau::calculPoids(void) {
    int x, y, idx;
    
    poids = 0;
    
    for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
            poids += modele[idx] * plateau[idx];
        }
    }
}

int CPlateau::getNextIdx(int idx) {
    int x = idx % NB_COLONNE;
    int y = idx / NB_COLONNE;
    
    do {
        x++;
        if(x == NB_COLONNE) {
            x = 0;
            y++;
        }
        
        idx = y * NB_COLONNE + x;
    }while(!modele[idx]);
    
    return idx;
}
    

CPlateau::CPlateau(unsigned char *modele, int idx) {
	memcpy(plateau, modele, NB_BILLE * sizeof(unsigned char));
	plateau[idx] = BILLE;
    
    calculPoids();
}

CPlateau::CPlateau(CPlateau * other, CCoup coup) {
	int depuis = coup.getDepuis();
	CCoup::ETypeCoup type = coup.getType();
	
	memcpy(plateau, other->plateau, NB_BILLE * sizeof(unsigned char));
	
	plateau[depuis] = BILLE;
	switch(type) {
		case CCoup::etcHaut:
			plateau[depuis-NB_COLONNE] = BILLE;
			plateau[depuis-NB_COLONNE*2] = VIDE;
			break;
		case CCoup::etcDroite:
			plateau[depuis+1] = BILLE;
			plateau[depuis+2] = VIDE;
			break;
		case CCoup::etcBas:
			plateau[depuis+NB_COLONNE] = BILLE;
			plateau[depuis+NB_COLONNE*2] = VIDE;
			break;
		case CCoup::etcGauche:
			plateau[depuis-1] = BILLE;
			plateau[depuis-2] = VIDE;
			break;
	}
	
	coups.push_back(coup);
	coups.insert(coups.end(), other->coups.begin(), other->coups.end());
    
    calculPoids();
}

CPlateau::~CPlateau(void) {
	coups.clear();
}

void CPlateau::print(int offsetX, int offsetY, unsigned char *pl) {
	int x, y, idx;
    
    if(pl == 0) {
        pl = plateau;
    }
	
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	attron(COLOR_PAIR(1));
    
    move(offsetY, offsetX);
    printw("Poids: %u", poids);
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(y == 0) {
				move(offsetY + 1, x + 2 + offsetX);
				printw("%c", 'A'+(char)x);
			}
			
			if(x == 0) {
				move(y + 2 + offsetY + 1, offsetX);
				printw("%d", y+1);
			}
			
			move(y + 2 + offsetY + 1, x + 2 + offsetX);
			
			if(pl[idx] == BILLE) {
				printw("o");
			}else if(pl[idx] == VIDE) {
				printw(" ");
			}
		}
	}
	
	attroff(COLOR_PAIR(1));
}

bool CPlateau::equal(CPlateau *other) {
	if(poids == other->poids) {
		int i, j;
		unsigned char test[NB_BILLE];
		
		memcpy(test, plateau, NB_BILLE * sizeof(unsigned char));

		for(i=0;i<2;i++) {
			for(j=0;j<4;j++) {
				if(memcmp(test, other->plateau, NB_BILLE * sizeof(unsigned char)) == 0) {
					return true;
				}
				
				rotate(test);
			}
			
			mirror(test);
		}	
	}
	
	return false;
}

bool CPlateau::inf(CPlateau *other) {
	return false;
}

std::list<CCoup> CPlateau::getNextCoups(int numCoup) {
	std::list<CCoup> coups;
	int x, y, idx, nbCoup;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == BILLE) {
				for(nbCoup=0;nbCoup<DIFFRENT_COUP;nbCoup++) {
					coups.push_back(fctCoup[numCoup](plateau, x, y, idx));
					numCoup = (numCoup + 1) % DIFFRENT_COUP;
				}
			}
		}
	}
	
	return coups;
}

std::list<CCoup> CPlateau::getCoups(void) {
    return coups;
}

void CPlateau::printVide(void) {
    int x, y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
            if(plateau[idx] == VIDE) {
                printf("Départ: %c,%d\n", x + 'A', y + 1);
                return;
            }
        }
    }
}

unsigned CPlateau::getPoids() {
	return poids;
}

void CPlateau::testModele(int nbBille) {
    int *idxBille = new int[nbBille];
    int ib;
    
    memset(idxBille, 255, nbBille * sizeof(int));
    
    while(idxBille[0] < NB_BILLE - 5) {
        idxBille[0] = CPlateau::getNextIdx(idxBille[0]);
        memset(idxBille+1, 255, (nbBille - 1) * sizeof(int));
        
        idxBille[1] = CPlateau::getNextIdx(idxBille[0]);
        do {
            int poids = 0;
            
            for(ib=0;ib<nbBille;ib++) {
                poids += modele[idxBille[ib]];
            }
            
            printf("%d %d %d\n", idxBille[0], idxBille[1], poids);
            idxBille[1] = CPlateau::getNextIdx(idxBille[1]);
        }while(idxBille[1] < NB_BILLE - 3);
        
        getchar();
    }
    
    delete[] idxBille;
}

CCoup haut(unsigned char *plateau, int x, int y, int idx) {
	if(y < NB_LIGNE - 2 && plateau[idx + NB_COLONNE] == VIDE && plateau[idx + NB_COLONNE * 2] == VIDE) {
		return CCoup(CCoup::etcHaut, idx + NB_COLONNE * 2);
	} 
	
	return CCoup();
}

CCoup droite(unsigned char *plateau,int x, int y, int idx) {
	if(x >= 2  && plateau[idx - 1] == VIDE && plateau[idx - 2] == VIDE) { //Droite
		return CCoup(CCoup::etcDroite, idx - 2);
	}
	
	return CCoup();
}

CCoup bas(unsigned char *plateau, int x, int y, int idx) {
	if(y >= 2 && plateau[idx - NB_COLONNE] == VIDE && plateau[idx - NB_COLONNE * 2] == VIDE) { //Bas
		return CCoup(CCoup::etcBas, idx - NB_COLONNE * 2);
	}
	
	return CCoup();
}

CCoup gauche(unsigned char *plateau, int x, int y, int idx) {
	if(x < NB_COLONNE - 2  && plateau[idx + 1] == VIDE && plateau[idx + 2] == VIDE) { //Gauche
		return CCoup(CCoup::etcGauche, idx + 2);
	}
	
	return CCoup();
}
