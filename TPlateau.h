#ifndef __TPLATEAU__
#define __TPLATEAU__

#include "SListCoup.h"

#define NB_COLONNE			9
#define NB_LIGNE			9
#define NB_BILLE			(NB_COLONNE*NB_LIGNE)
#define UNDEF				((unsigned char)255)
#define BILLE				((unsigned char)1)
#define VIDE				((unsigned char)0)

typedef unsigned char TPlateau[NB_BILLE];

void init(TPlateau plateau, int x, int y);
void print(TPlateau plateau);
void cpy(TPlateau dst, TPlateau src);
SListCoup * solve(TPlateau plateau);
void joue(TPlateau plateau, SCoup *coup);
void rotate(TPlateau plateau, TPlateau new);
void miror(TPlateau plateau, TPlateau new);

#endif //__TPLATEAU__