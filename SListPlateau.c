#include <stdlib.h>
#include <string.h>
#include "SListPlateau.h"

static SListPlateau * dernier(SListPlateau *list);

void pushPlateau(SListPlateau **list, TPlateau plateau) {
	SListPlateau *new = (SListPlateau *)malloc(sizeof(SListPlateau));
	
	cpy(new->plateau, plateau);
	new->next = 0;
	
	if(*list == 0) {
		*list = new;
	} else {
		dernier(*list)->next = new;
	}
}

int inListPlateau(SListPlateau *list, TPlateau plateau) {
	while(list != 0) {
		TPlateau test, roTest;
		int i = 0;
		
		cpy(test, list->plateau);
		
		do {
			if(!memcmp(test, plateau, NB_BILLE * sizeof(unsigned char))) {
				return 1;
			}
			
			rotate(test, roTest);
			cpy(test, roTest);
		}while(++i != 4);
		
		list = list->next;
	}
	
	return 0;
}

void deleteList(SListPlateau *list) {
	SListPlateau *next = list;
	
	while(next != 0) {
		next = list->next;
		free(list);
		list = next;
	}
}

SListPlateau * dernier(SListPlateau *list) {
	if(list != 0) {
		SListPlateau *p = list;
		
		while(p->next != 0) {
			p = p->next;
		};
		
		return p;
	}
	
	return 0;
}
