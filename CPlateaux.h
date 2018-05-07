#ifndef __CPlateaux__
#define __CPlateaux__

#include <list>
#include "CPlateau.h"

class CPlateaux {
private:
    std::list<CPlateau *> list;
public:
    ~CPlateaux(void);
    bool add(CPlateau *);
    std::list<CPlateau *>::iterator begin(void);
    std::list<CPlateau *>::iterator end(void);
    unsigned long size(void);
};

#endif // __CPlateaux__
