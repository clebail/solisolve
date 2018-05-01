#ifndef __SLISTPLATEAU__
#define __SLISTPLATEAU__

#include "TPlateau.h"

typedef struct _SListPlateau {
	TPlateau plateau;
	struct _SListPlateau *next;
}SListPlateau;

void pushPlateau(SListPlateau **list, TPlateau plateau);
int inListPlateau(SListPlateau *list, TPlateau plateau);
void deleteList(SListPlateau *list);


#endif //__SLISTPLATEAU__