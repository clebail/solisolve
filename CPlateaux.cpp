#include <algorithm>
#include "CPlateaux.h"
#include "common.h"

bool CPlateaux::find(CPlateau *plateau, int debut, int fin, int &idx) {
	int milieu;
	
	milieu = (fin + debut) / 2;

	dout << "milieu: " << milieu << " debut: " << debut << " fin: " << fin << std::endl;
	
	if(size == 0 || debut > fin) {
		idx = debut;
		
		return false;
	}
	
	dout << "poids milieu: " << plx[milieu]->getPoids() << " poids autre: " << plateau->getPoids() << std::endl;
	if(plx[milieu]->getPoids() == plateau->getPoids()) {
		int p = milieu;
		while(p >= 0 && plx[p]->getPoids() == plateau->getPoids()) {
			milieu = p;
			p--;
		}
		
		idx = milieu;
		
		return true;
	}else if(plateau->getPoids() < plx[milieu]->getPoids()) {
		return find(plateau, debut, milieu-1, idx);
	} else {
		return find(plateau, milieu+1, fin, idx);
	}
}

void CPlateaux::clear(void) {
	int i;
	
	for(i=0;i<size;i++) {
		delete plx[i];
	}
}

CPlateaux::CPlateaux(void) {
	plx = 0;
	size = 0;
}

CPlateaux::~CPlateaux(void){
    clear();
	
	delete plx;
	
	plx = 0;
	size = 0;
}

bool CPlateaux::add(CPlateau *plateau) {
    CPlateau **newPlx;
	int idx, i, newI;
	
	if(find(plateau, 0, size-1, idx)) {
		dout << "Plateaux de même poids " << plateau->getPoids() << std::endl;
		i=idx;
		while(i < size && plateau->getPoids() == plx[i]->getPoids()) {
			if(plateau->equal(plx[i])) {
				dout << "1 plateau équivalent trouvé" << std::endl;
				return false;
			}
			i++;
		}
	}
	
	dout << "Ajout du plateau à l'index " << idx << std::endl;
	newPlx = new CPlateau*[size+1];
	newPlx[idx] = plateau;
	for(i=newI=0;i<size;i++,newI++) {
		if(i == idx) {
			newI++;
		}
		newPlx[newI] = plx[i];
	}
	delete[] plx;
	size++;
	plx = newPlx;
	
	return true;
}

int CPlateaux::getSize(void) {
    return size;
}

CPlateau * CPlateaux::get(int idx) {
	return plx[idx];
}
