#include <algorithm>
#include "CPlateaux.h"

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
