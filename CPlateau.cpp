#include <string.h>
#include <ncurses.h>
#include "CPlateau.h"

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

CPlateau::CPlateau(unsigned char *modele, int idx) {
	memcpy(plateau, modele, NB_BILLE * sizeof(unsigned char));
	plateau[idx] = BILLE;
}

CPlateau::CPlateau(const CPlateau& other, CCoup coup) {
	int depuis = coup.getDepuis();
	CCoup::ETypeCoup type = coup.getType();
	
	memcpy(plateau, other.plateau, NB_BILLE * sizeof(unsigned char));
	
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
	coups.insert(coups.end(), other.coups.begin(), other.coups.end());
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
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(y == 0) {
				move(offsetY, x + 2 + offsetX);
				printw("%c", 'A'+(char)x);
			}
			
			if(x == 0) {
				move(y + 2 + offsetY, offsetX);
				printw("%d", y+1);
			}
			
			move(y + 2 + offsetY, x + 2 + offsetX);
			
			if(pl[idx] == BILLE) {
				printw("o");
			}else if(pl[idx] == VIDE) {
				printw(" ");
			}
		}
	}
	
	attroff(COLOR_PAIR(1));
}

bool CPlateau::operator < (const CPlateau& other) {
    int i, j;
    unsigned char test[NB_BILLE];
    
    memcpy(test, plateau, NB_BILLE * sizeof(unsigned char));

    for(i=0;i<2;i++) {
        for(j=0;j<4;j++) {
            
            if(memcmp(test, other.plateau, NB_BILLE * sizeof(unsigned char)) == 0) {
                return false;
            }
            
            rotate(test);
        }
        
        mirror(test);
    }
	
	return memcmp(plateau, other.plateau, NB_BILLE * sizeof(unsigned char)) < 0;
}

std::list<CCoup> CPlateau::getNextCoups(void) {
	std::list<CCoup> coups;
	int x, y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == BILLE) {
				if(y < NB_LIGNE - 2 && plateau[idx + NB_COLONNE] == VIDE && plateau[idx + NB_COLONNE * 2] == VIDE) { //Haut
					CCoup coup(CCoup::etcHaut, idx + NB_COLONNE * 2);
					coups.push_back(coup);
				} 
				
				if(x >= 2  && plateau[idx - 1] == VIDE && plateau[idx - 2] == VIDE) { //Droite
					CCoup coup(CCoup::etcDroite, idx - 2);
					coups.push_back(coup);
				} 
				
				if(y >= 2 && plateau[idx - NB_COLONNE] == VIDE && plateau[idx - NB_COLONNE * 2] == VIDE) { //Bas
					CCoup coup(CCoup::etcBas, idx - NB_COLONNE * 2);
					coups.push_back(coup);
				} 
				
				if(x > NB_COLONNE - 2  && plateau[idx + 1] == VIDE && plateau[idx + 2] == VIDE) { //Gauche
					CCoup coup(CCoup::etcGauche, idx + 2);
					coups.push_back(coup);
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
