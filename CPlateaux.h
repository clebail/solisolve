#ifndef __CPlateaux__
#define __CPlateaux__

#include <set>
#include "CPlateau.h"
#include "SPlateauCmp.h"

class CPlateaux : public std::set<CPlateau *, SPlateauCmp> {
private:
	void clear(void);
public:
	~CPlateaux(void);
	bool add(CPlateau *);
};

#endif // __CPlateaux__
