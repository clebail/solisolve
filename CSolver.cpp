#include <ncurses.h>
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

void CSolver::passe(int nbBille) {
	std::list<CFeuille *>::iterator i;
	
	for(i=feuilles.begin();i!=feuilles.end();i++) {
		CFeuille *f = *i;
		
		if(!f->process(nbBille)) {
			delete f;
		}
	}
}

CSolver::CSolver(void) {
	int x, y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(modele[idx] == VIDE) {
				CPlateau *plateau = new CPlateau(modele, x, y);
				
				feuilles.push_back(new CFeuille(plateau));
			}
		}
	}
}

CSolver::~CSolver(void) {
}

int CSolver::getNbFeuille(void) {
	return feuilles.size();
}

void CSolver::process(void) {
	int i;
	
	for(i=2;i<=MAX_BILLE;i++) {
		passe(i);
	}
}
