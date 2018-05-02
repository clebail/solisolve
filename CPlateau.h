#ifndef __CPlateau__
#define __CPlateau__

#include <list>
#include "CCoup.h"

#define NB_COLONNE			9
#define NB_LIGNE			9
#define NB_BILLE			(NB_COLONNE*NB_LIGNE)
#define UNDEF				((unsigned char)255)
#define BILLE				((unsigned char)1)
#define VIDE				((unsigned char)0)
#define MAX_BILLE			56

class CPlateau {
private:
	unsigned char plateau[NB_BILLE];
public:
	CPlateau(unsigned char * modele, int x, int y);
	CPlateau(const CPlateau& other, CCoup coup);
	void print(void);
};

#endif //__CPlateau__
