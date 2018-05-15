#include <stdio.h>
#include <string.h>
#include <CPlateau.h>

#define MAX_LINE			256

typedef struct _SId {
	char x;
	int y;
}SId;

typedef struct _SCommand {
	char cmd;
	SId id;
	CCoup::ETypeCoup type;
}SCommand;

int getIdx(char x, int y);
void initPlateau(SId * plateau, SId * depart);

static unsigned char modele[NB_BILLE] = {
	255, 255, 1, 1, 1, 255, 255, 
	255, 1, 1, 1, 1, 1, 255, 
	1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	255, 1, 1, 1, 1, 1, 255,
	255, 255, 1, 1, 1, 255, 255, 
};

int main(int argc, char *argv[]) {
	FILE *f;
	
	if(argc != 2) {
		printf("Usage: anim <filename>\n");
		
		return 1;
	}
	
	f = fopen(argv[1], "r");
	if(f != 0) {
		char line[MAX_LINE];
		SCommand commande;
		SId plateau[NB_BILLE];
		
		while(!feof(f)) {
			fgets(line, MAX_LINE, f);
			
			sscanf(line, "%c %c,%d %d", &commande.cmd, &commande.id.x, &commande.id.y, (int *)&commande.type);
			
			if(commande.type == 'D') {
				initPlateau(plateau, &commande.id);
			}else {
			}
		}
		
		fclose(f);
	}else {
		printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		
		return 1;
	}
	
    return 0;
}

int getIdx(char x, int y) {
	return (y - 1) * NB_COLONNE + x - 'A';
}

void initPlateau(SId * plateau, SId * depart) {
	int x, y, idx;
	
	memset(plateau, 0, sizeof(SId) * NB_BILLE);
	
	for(y=idx=0;y<NB_LIGNE;y++) {
		for(x=0;x<NB_COLONNE;x++,idx++) {
			if(modele[idx] == BILLE) {
				plateau[idx].x = x + 'A';
				plateau[idx].y = y + 1;
			}
		}
	}
}

