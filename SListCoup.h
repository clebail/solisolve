#ifndef __SLISTCOUP__
#define __SLISTCOUP__

#include "SCoup.h"

typedef struct _SListCoup {
	SCoup *coup;
	struct _SListCoup *next;
}SListCoup;

int taille(SListCoup *list);
void pushCoup(SListCoup **list, SCoup *coup);
SCoup *popCoup(SListCoup **list);


#endif //__SLISTCOUP__