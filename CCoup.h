#ifndef __CCOUP__
#define __CCOUP__

#include <list>

class CCoup {
public:
	typedef enum {etcHaut, etcDroite, etcBas, etcGauche} ETypeCoup;
private:
	CCoup::ETypeCoup type;
	int depuis;
	bool _isNull;
public:
	CCoup(void);
	CCoup(CCoup::ETypeCoup type, int depuis);
	const CCoup::ETypeCoup& getType(void);
	const int& getDepuis(void);
    void print(void);
	bool isNull(void);
};

#endif //__CCOUP__
