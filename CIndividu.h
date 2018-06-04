#ifndef __CINDIVIDU__
#define __CINDIVIDU__

class CIndividu {
public:
	virtual ~CIndividu(void) {}
	virtual void init(void) = 0;
	virtual int getScore(void) = 0;
	virtual void operator = (const CIndividu& other) = 0;
	static CIndividu *newIntance(void) { return 0; }
};

#endif //__CINDIVIDU__