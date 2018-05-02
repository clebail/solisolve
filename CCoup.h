#ifndef __CCOUP__
#define __CCOUP__

#include <list>

class CCoup {
public:
	typedef enum {etcHaut, etcDroite, etcBas, etcGauche} ETypeCoup;
private:
	CCoup::ETypeCoup type;
	int depuis;
public:
	CCoup(CCoup::ETypeCoup type, int depuis);
	const CCoup::ETypeCoup& getType(void);
	const int& getDepuis(void);
};

#endif //__CCOUP__
