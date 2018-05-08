#include <algorithm>
#include "CPlateaux.h"

/*CPlateau * CPlateaux::find(CPlateau *plateau, std::list<CPlateau *>::iterator first, std::list<CPlateau *>::iterator end) {
	std::list<CPlateau *>::iterator milieu;
	
	if(first == end) {
		return 0;
	}
	
	milieu = (end - first) / 2;
	if((*milieu)->equal(plateau)) {
		std::list<CPlateau *>::iterator p = milieu;
		while((*(--p))->equal(plateau)) {
			milieu = p;
		}
		return *milieu;
	}
	
	return 0;
}*/

CPlateaux::~CPlateaux(void){
    std::list<CPlateau *>::iterator i;
    
    for(i=list.begin();i!=list.end();i++) {
        CPlateau *p = *i;
        
        delete p;
    }
    
    list.clear();
}

bool CPlateaux::add(CPlateau *plateau) {
    std::list<CPlateau *>::iterator it;
    
    for(it=list.begin();it!=list.end();it++) {
        CPlateau *p = *it;
        
        if(p->equal(plateau)) {
            return false;
        }
    }
    
    list.push_back(plateau);
    
    return true;
}

std::list<CPlateau *>::iterator CPlateaux::begin(void) {
    return list.begin();
}

std::list<CPlateau *>::iterator CPlateaux::end(void) {
    return list.end();
}

unsigned long CPlateaux::size(void) {
    return list.size();
}
