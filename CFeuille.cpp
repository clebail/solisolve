#include "CFeuille.h"

CFeuille::CFeuille(CPlateau *plateau, CFeuille *resultat, CCoup *coup, int nbBille) {
	this->plateau = plateau;
	this->resultat = resultat;
	this->coup = coup;
	this->nbBille = nbBille;
}

CFeuille::~CFeuille(void) {
	std::list<CFeuille *>::iterator i;
	
	if(resultat != 0) {
		delete resultat;
	}
	
	if(coup != 0) {
		delete coup;
	}
	
	delete plateau;
	
	for(i=ancetres.begin();i!=ancetres.end();i++) {
		CFeuille *f = *i;
		delete f;
	}
}

bool CFeuille::process(int nbBille) {
	if(ancetres.size() != 0) {
		std::list<CFeuille *>::iterator i;
	
		for(i=ancetres.begin();i!=ancetres.end();i++) {
			CFeuille *f = *i;
			if(!f->process(nbBille)) {
				delete f;
			}
		}
	}
	
	return processAncetres(nbBille);
}

bool CFeuille::processAncetres(int nbBille) {
	int x, y, idx;
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(plateau[idx] == BILLE) {
			}
		}
	}
	
	return true;
}
