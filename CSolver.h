#ifndef __CSOLVER__
#define __CSOLVER__

#include <set>
#include "CPlateau.h"
#include "SPlateauCmp.h"

class CSolver {
private:
	std::set<CPlateau *, SPlateauCmp> plateaux;
	
	void init(void);
	bool addPlateauIfNotExistst(std::set<CPlateau *, SPlateauCmp> &plateaux, CPlateau *plateau);
	void clear(void);
public:
	CSolver(void);
	~CSolver(void);
	int getNbPlateaux(void);
	void process(void);
};

#endif //__CSOLVER__
