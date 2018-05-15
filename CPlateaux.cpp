#include <algorithm>
#include "CPlateaux.h"
#include "common.h"

void CPlateaux::clear(void) {
	std::set<CPlateau *, SPlateauCmp>::iterator it;
	
	for(it=begin();it!=end();it++) {
		delete *it;
	}
	
	std::set<CPlateau *, SPlateauCmp>::clear();
}

CPlateaux::~CPlateaux(void){
    clear();
}

bool CPlateaux::add(CPlateau *plateau) {
	auto search = find(plateau);
	
	if(search == end()) {
		insert(plateau);
		return true;
	}
	
	return false;
}
