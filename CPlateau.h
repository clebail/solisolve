#ifndef __CPlateau__
#define __CPlateau__

#include "CGene.h"

#define NB_COLONNE			7
#define NB_LIGNE			7
#define NB_BILLE			(NB_COLONNE*NB_LIGNE)
#define UNDEF				((unsigned char)255)
#define BILLE				((unsigned char)1)
#define VIDE				((unsigned char)0)
#define MAX_BILLE			37
#define DIFFRENT_COUP		4

class CPlateau : public CGene {
private:
	unsigned char plateau[NB_BILLE];
	int nbTrou;
public:
	void init(int nbTrou);
	int getNbTrou(void);
	void from(CPlateau * other);
};

#endif //__CPlateau__
