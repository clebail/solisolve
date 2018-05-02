#include <string.h>
#include <ncurses.h>
#include "CPlateau.h"

CPlateau::CPlateau(unsigned char *modele, int x, int y) {
	memcpy(plateau, modele, NB_BILLE * sizeof(unsigned char));
	plateau[y * NB_COLONNE + x] = BILLE;
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
}

void CPlateau::print(void) {
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
}
