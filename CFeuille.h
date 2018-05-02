#ifndef __CFEUILLE__
#define __CFEUILLE__

#include <list>
#include "CPlateau.h"
#include "CCoup.h"

class CFeuille {
private:
	CPlateau *plateau;
	CFeuille *resultat;
	CCoup *coup;
	std::list<CFeuille *> ancetres;
	int nbBille;
public:
	CFeuille(CPlateau *plateau, CFeuille *resultat = 0, CCoup *coup = 0, int nbBille = 1);
	~CFeuille(void);
	bool process(int nbBille);
	bool processAncetres(int nbBille);
};

#endif //__CFEUILLE__
