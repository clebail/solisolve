#ifndef __CINDIVIDU__
#define __CINDIVIDU__

class CIndividu {
public:
	virtual ~CIndividu(void) {}
	virtual void init(void) = 0;
	virtual int getScore(void) = 0;
	virtual void calculScore(void) = 0;
	virtual void mute(int idxGene) = 0;
	virtual void from(CIndividu *i1, CIndividu *i2, int seuil) = 0;
};

#endif //__CINDIVIDU__
