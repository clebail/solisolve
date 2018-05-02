#ifndef __CSOLVER__
#define __CSOLVER__

#include <list>
#include "CFeuille.h"

class CSolver {
private:
	std::list<CFeuille *> feuilles;
	
	void passe(int nbBille);
public:
	CSolver(void);
	~CSolver(void);
	int getNbFeuille(void);
	void process(void);
};

#endif //__CSOLVER__
