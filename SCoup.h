#ifndef __SCOUP__
#define __SCOUP__

typedef enum {etcHaut, etcDroite, etcBas, etcGauche} ETypeCoup;

typedef struct _SCoup {
	ETypeCoup type;
	int depuis;
} SCoup;

void printCoup(SCoup *coup);

#endif //__SCOUP__
