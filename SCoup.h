#ifndef __SCOUP__
#define __SCOUP__

typedef enum {etcHaut, etcDroite, etcBas, etcGauche} ETypeCoup;

typedef struct _SCoup {
	ETypeCoup type;
	int depuis;
} SCoup;

#endif //__SCOUP__