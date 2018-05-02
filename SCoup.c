#include <stdio.h>
#include <string.h>
#include "SCoup.h"
#include "TPlateau.h"

void printCoup(SCoup *coup) {
	char x;
	int y;
	char sens[1024];
	
	x = coup->depuis % NB_COLONNE + 'A';
	y = coup->depuis / NB_COLONNE + 1;
	
	switch(coup->type) {
		case etcHaut:
			strcpy(sens, "le haut");
			break;
		case etcDroite:
			strcpy(sens, "la droite");
			break;
		case etcBas:
			strcpy(sens, "le bas");
			break;
		case etcGauche:
			strcpy(sens, "la gauche");
			break;
	}
	
	printf("Coup depuis %c,%d vers %s\n", x, y, sens);
}
