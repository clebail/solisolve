#ifndef __CSOLVER__
#define __CSOLVER__

#include "CPlateaux.h"
#include "CPlateau.h"

class CSolver {
private:
	CPlateaux *plateaux;
	bool visu;

	void init(void);
	bool addPlateauIfNotExistst(CPlateaux *plateaux, CPlateau *plateau);
	void clearPlateaux(void);
public:
	CSolver(bool visu = false);
	~CSolver(void);
	int getNbPlateaux(void);
	void process(void);
};

#endif //__CSOLVER__
