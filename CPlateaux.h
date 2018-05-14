#ifndef __CPlateaux__
#define __CPlateaux__

#include <list>
#include "CPlateau.h"

class CPlateaux {
private:
	CPlateau **plx;
	int size;
	
	bool find(CPlateau *plateau, int debut, int fin, int& idx);
	void clear(void);
public:
	CPlateaux(void);
	~CPlateaux(void);
	bool add(CPlateau *);
	int getSize(void);
	CPlateau * get(int idx);
};

#endif // __CPlateaux__
