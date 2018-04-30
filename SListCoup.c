#include <stdlib.h>
#include "SListCoup.h"

static SListCoup * dernier(SListCoup *list);

int taille(SListCoup *list) {
	SListCoup *p = list;
	int nb = 0;
	
	while(p != 0) {
		p = p->next;
		nb++;
	}
	
	return nb;
}

void push(SListCoup **list, SCoup *coup) {
	SListCoup *new = (SListCoup *)malloc(sizeof(SListCoup));
	
	new->coup = coup;
	new->next = 0;
	
	if(*list == 0) {
		*list = new;
	} else {
		dernier(*list)->next = new;
	}
}

SCoup * pop(SListCoup **list) {
	SCoup *result = (*list)->coup;
	SListCoup *item = (*list);
	
	*list = (*list)->next;
	
	free(item);
	
	return result;
}

SListCoup * dernier(SListCoup *list) {
	if(list != 0) {
		SListCoup *p = list;
		
		while(p->next != 0) {
			p = p->next;
		};
		
		return p;
	}
	
	return 0;
}