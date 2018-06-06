#ifndef __CPlateaux__
#define __CPlateaux__

#include "CIndividu.h"
#include "CPlateau.h"

class CPlateaux : public CIndividu {
private:
	int score;
	CPlateau plateaux[MAX_BILLE-1];
protected:
	CGene * getGene(int idx);
public:
	void init(void);
	int getScore(void);
	void calculScore(void);
	void mute(int idxGene);
	void from(CIndividu *i1, CIndividu *i2, int seuil);
};

#endif // __CPlateaux__
