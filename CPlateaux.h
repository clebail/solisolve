#ifndef __CPlateaux__
#define __CPlateaux__

#include <list>
#include "CIndividu.h"
#include "CPlateau.h"

class CPlateaux : public CIndividu {
private:
	int score;
	CPlateau plateaux[MAX_BILLE-1];
	std::list<CCoup> coups;
	
	void getCoords(int idx, char& x, int& y);
protected:
	CGene * getGene(int idx);
public:
	void init(void);
	int getScore(void);
	void calculScore(void);
	void mute(int idxGene);
	void from(CIndividu *i1, CIndividu *i2, int seuil);
	void print(void);
};

#endif // __CPlateaux__
