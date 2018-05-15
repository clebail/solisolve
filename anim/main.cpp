#include <stdio.h>
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

int main(int argc, char *argv[]) {
	FILE *f;
	
	if(argc != 2) {
		printf("Usage: anim <filename>\n");
	}
	
	f = fopen(argv[1], "r");
	if(f != 0) {
		char line[MAX_LINE];
		SCommand commande;
		
		while(!feof(f)) {
			fgets(line, MAX_LINE, f);
			
			sscanf(line, "%c %c,%d %d", &commande.cmd, &commande.id.x, &commande.id.y, (int *)&commande.type);
			
			if(commande.type == 'D') {
				
			}else {
			}
		}
		
		fclose(f);
	}else {
		printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
	}
	
    return 0;
}

int getIdx(char x, int y) {
	return (y - 1) * NB_COLONNE + x - 'A';
}
