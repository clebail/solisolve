#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "TPlateau.h"
#include "SListPlateau.h"

static TPlateau modele = {
	255, 255, 255, 1, 1, 1, 255, 255, 255,
	255, 255, 1, 1, 1, 1, 1, 255, 255, 
	255, 1, 1, 1, 1, 1, 1, 1, 255,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	255, 1, 1, 1, 1, 1, 1, 1, 255,
	255, 255, 1, 1, 1, 1, 1, 255, 255, 
	255, 255, 255, 1, 1, 1, 255, 255, 255,
};

static SListPlateau *mauvais[NB_BILLE];
static int nbMauvais = 0;
static int nbTestMauvais = 0;

static int cherche(TPlateau plateau, SListCoup **coupsOk);
static int gagne(TPlateau plateau);
static SListCoup * getCoups(TPlateau plateau);
static void pushMauvais(TPlateau plateau);

void init(TPlateau plateau, int x, int y) {
	cpy(plateau, modele);
	plateau[y*NB_COLONNE+x] = VIDE;
	
	memset(mauvais, 0, sizeof(SListPlateau *) * NB_BILLE);
}

void print(TPlateau plateau) {
	int x,y, idx;
	
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	attron(COLOR_PAIR(1));
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(y == 0) {
				move(0, x+2);
				printw("%c", 'A'+(char)x);
			}
			
			if(x == 0) {
				move(y+2, 0);
				printw("%d", y+1);
			}
			
			move(y+2, x+2);
			
			if(plateau[idx] == BILLE) {
				printw("o");
			}else if(plateau[idx] == VIDE) {
				printw(" ");
			}
		}
	}
	
	attroff(COLOR_PAIR(1));
	refresh();
}

void cpy(TPlateau dst, TPlateau src) {
	memcpy(dst, src, NB_BILLE * sizeof(unsigned char));
}

SListCoup * solve(TPlateau plateau) {
	SListCoup *coupOk = 0;
	
	cherche(plateau, &coupOk);
	
	return coupOk;
}

void joue(TPlateau plateau, SCoup *coup) {
	plateau[coup->depuis] = VIDE;
	switch(coup->type) {
		case etcHaut:
			plateau[coup->depuis-NB_COLONNE] = VIDE;
			plateau[coup->depuis-NB_COLONNE*2] = BILLE;
			break;
		case etcDroite:
			plateau[coup->depuis+1] = VIDE;
			plateau[coup->depuis+2] = BILLE;
			break;
		case etcBas:
			plateau[coup->depuis+NB_COLONNE] = VIDE;
			plateau[coup->depuis+NB_COLONNE*2] = BILLE;
			break;
		case etcGauche:
			plateau[coup->depuis-1] = VIDE;
			plateau[coup->depuis-2] = BILLE;
			break;
	}
}

void rotate(TPlateau plateau, TPlateau new) {
	int x, y, idx, newIdx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			newIdx = (NB_LIGNE - x - 1) * NB_COLONNE + y;
			new[newIdx] = plateau[idx];
		}
	}
}

int getNbBille(TPlateau plateau) {
	int x, y, idx, nbBille = 0;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == BILLE) {
				nbBille++;
			}
		}
	}
	
	return nbBille;
}


void miror(TPlateau plateau, TPlateau new) {
	int x, y, idx, newIdx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			newIdx = (NB_LIGNE - y - 1) * NB_COLONNE + x;
			new[newIdx] = plateau[idx];
		}
	}
}

int cherche(TPlateau plateau, SListCoup **coupsOk) {
	SListCoup *coups;
	
	if(gagne(plateau)) {
		int i;
		
		for(i=0;i<NB_BILLE;i++) {
			deleteList(mauvais[i]);
		}
		
		return 1;
	}
	
	coups = getCoups(plateau);
	
	if(coups == 0) {
		print(plateau);
		pushMauvais(plateau);
	}else {
		while(coups != 0) {
			int nbBille = 0;
			TPlateau nextPlateau;
			SCoup *coup = popCoup(&coups);
			
			cpy(nextPlateau, plateau);
			
			joue(nextPlateau, coup);
			//print(nextPlateau);
			
			nbBille = getNbBille(nextPlateau);
			
			if(!inListPlateau(mauvais[nbBille], nextPlateau)) {
				if(cherche(nextPlateau, coupsOk)) {
					pushCoup(coupsOk, coup);
					
					return 1;
				}
			}else {
				nbTestMauvais++;
				
				move(0, 40);
				printw("mauvais déjà testé: %10d", nbTestMauvais);
				refresh();
			}
			
			free(coup);
		}
		
		print(plateau);
		pushMauvais(plateau);
	}
	
	return 0;
}

int gagne(TPlateau plateau) {
	int x,y, idx, nbBille;
	
	for(y=idx=nbBille=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == BILLE) {
				if(nbBille == 1) {
					return 0;
				}
				nbBille++;
			}
		}
	}
	
	return 1;
}

SListCoup * getCoups(TPlateau plateau) {
	SListCoup *lCoup = 0;
	int x,y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == VIDE) {
				if(y < NB_LIGNE - 2 && plateau[idx+NB_COLONNE] == BILLE && plateau[idx+NB_COLONNE*2] == BILLE) {
					SCoup *coup = (SCoup *)malloc(sizeof(SCoup));
					
					coup->type = etcHaut;
					coup->depuis = idx+NB_COLONNE*2;
					
					pushCoup(&lCoup, coup);
				}
				
				if(x >=2  &&  plateau[idx-1] == BILLE && plateau[idx-2] == BILLE) {
					SCoup *coup = (SCoup *)malloc(sizeof(SCoup));
					
					coup->type = etcDroite;
					coup->depuis = idx-2;
					
					pushCoup(&lCoup, coup);
				}
				
				if(y >= 2 && plateau[idx-NB_COLONNE] == BILLE && plateau[idx-NB_COLONNE*2] == BILLE) {
					SCoup *coup = (SCoup *)malloc(sizeof(SCoup));
					
					coup->type = etcBas;
					coup->depuis = idx-NB_COLONNE*2;
					
					pushCoup(&lCoup, coup);
				}
				
				if(x < NB_COLONNE - 2 &&  plateau[idx+1] == BILLE && plateau[idx+2] == BILLE) {
					SCoup *coup = (SCoup *)malloc(sizeof(SCoup));
					
					coup->type = etcGauche;
					coup->depuis = idx+2;
					
					pushCoup(&lCoup, coup);
				}
			}
		}
	}
	
	return lCoup;
}

void pushMauvais(TPlateau plateau) {
	int nbBille = getNbBille(plateau);
	pushPlateau(&mauvais[nbBille], plateau);
	nbMauvais++;
	
	move(0, 20);
	printw("mauvais: %6d", nbMauvais);
	refresh();
}


