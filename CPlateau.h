#ifndef __CPlateau__
#define __CPlateau__

#include <list>
#include <string>
#include "CCoup.h"

#define NB_COLONNE			9
#define NB_LIGNE			9
#define NB_BILLE			(NB_COLONNE*NB_LIGNE)
#define UNDEF				((unsigned char)255)
#define BILLE				((unsigned char)1)
#define VIDE				((unsigned char)0)
#define MAX_BILLE			57

class CPlateau {
private:
	unsigned char plateau[NB_BILLE];
	std::list<CCoup> coups;
	
	void mirror(unsigned char *pl = 0);
	void rotate(unsigned char *pl = 0);
	void swap(unsigned char *c1, unsigned char *c2);
public:
	CPlateau(unsigned char * modele, int idx);
	CPlateau(const CPlateau& other, CCoup coup);
	~CPlateau(void);
	void print(int offsetX = 0, int offsetY = 0, unsigned char *pl = 0);
	bool operator < (const CPlateau& other);
	std::list<CCoup> getNextCoups(void);
    std::list<CCoup> getCoups(void);
    void printVide(void);
};

#endif //__CPlateau__
