#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "CPlateau.h"

static unsigned char modele[NB_BILLE] = {
	255, 255, 1, 1, 1, 255, 255, 
	255, 1, 1, 1, 1, 1, 255, 
	1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 
	255, 1, 1, 1, 1, 1, 255,
	255, 255, 1, 1, 1, 255, 255, 
};

void CPlateau::init(int nbTrou) {
	int nb = 0;
	
	memcpy(plateau, modele, sizeof(unsigned char) * NB_BILLE);
	
	while(nb != nbTrou) {
		int idx = rand() % NB_BILLE;
		
		if(plateau[idx] == BILLE) {
			plateau[idx] = VIDE;
			trous.insert(idx);
			nb++;
		}
	}
	
	this->nbTrou = nbTrou;
}

int CPlateau::getNbTrou(void) {
	return nbTrou;
}

void CPlateau::from(CPlateau * other) {
	memcpy(plateau, other->plateau, sizeof(unsigned char) * NB_BILLE);
	trous.clear();
	trous.insert(other->trous.begin(), other->trous.end()); 
}

bool CPlateau::isNext(CPlateau * other, CCoup& coup) {
	std::set<int>::iterator itTrou;
	int diff[2] = { -1, -1 };
	int curDiff = 0;
	
	for(itTrou = trous.begin();itTrou != trous.end();itTrou++) {
		if(other->trous.find(*itTrou) != other->trous.end()) {
			if(curDiff < 2) {
				diff[curDiff++] = *itTrou;
			}
		}
	}
	
	if(curDiff != 2) {
		return false;
	}
	
	if(diff[0] != diff[1] - 1 && diff[0] != diff[1] - NB_COLONNE) {
		return false;
	}
	
	if(diff[0] == diff[1] - 1) {
		if(diff[0] % NB_COLONNE != 0 && other->trous.find(diff[0] - 1) != other->trous.end()) {
			coup = CCoup(CCoup::etcGauche, diff[1]);
			return true;
		}
		
		if(diff[1] % NB_COLONNE != NB_COLONNE - 1 && other->trous.find(diff[1] + 1) != other->trous.end()) {
			coup = CCoup(CCoup::etcDroite, diff[0]);
			return true;
		}
	} else {
		if(diff[0] / NB_COLONNE != 0 && other->trous.find(diff[0] - NB_COLONNE) != other->trous.end()) {
			coup = CCoup(CCoup::etcHaut, diff[1]);;
			return true;
		}
		
		if(diff[1] % NB_COLONNE != NB_COLONNE - 1 && other->trous.find(diff[1] + NB_COLONNE) != other->trous.end()) {
			coup = CCoup(CCoup::etcBas, diff[0]);
			return true;
		}
	}
	
	return false;
}

const std::set<int> CPlateau::getTrous(void) {
	return trous;
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
