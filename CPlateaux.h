#ifndef __CPlateaux__
#define __CPlateaux__

#include <list>
#include "CPlateau.h"

class CPlateaux {
private:
    std::list<CPlateau *> list;
	//CPlateau * find(CPlateau *, std::list<CPlateau *>::iterator first, std::list<CPlateau *>::iterator end);
public:
    ~CPlateaux(void);
    bool add(CPlateau *);
    std::list<CPlateau *>::iterator begin(void);
    std::list<CPlateau *>::iterator end(void);
    unsigned long size(void);
};

#endif // __CPlateaux__
