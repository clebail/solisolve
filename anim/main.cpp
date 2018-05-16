#include <stdio.h>
#include <string.h>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <CPlateau.h>

#define MAX_LINE			256
#define CASE_HEIGHT			50
#define MARGIN_HEIGHT		35
#define CASE_WIDTH			50
#define MARGIN_WIDTH		35

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
	FILE *fr;
	
	if(argc != 2) {
		printf("Usage: anim <filename>\n");
		
		return 1;
	}
	
	fr = fopen(argv[1], "r");
	if(fr != 0) {
		char line[MAX_LINE];
		SId plateau[NB_BILLE];
		std::list<std::string> result;
		std::list<std::string>::iterator it;
		
		while(!feof(fr)) {
			fgets(line, MAX_LINE, fr);
			if(!feof(fr)) {
				SCommand commande;
				
				sscanf(line, "%c %c,%d %d", &commande.cmd, &commande.id.x, &commande.id.y, (int *)&commande.type);
				
				if(commande.cmd == 'D') {
					std::stringstream ss;
					
					ss << "\t{ property: 'opacity', value: '0', id: '#circle" << commande.id.x << commande.id.y << "', duration: 200 }," << std::endl;
					
					initPlateau(plateau, &commande.id);
					result.push_back(ss.str());
				}else if(commande.cmd == 'M') {
					std::stringstream ss;
					SId *id = &plateau[getIdx(commande.id.x, commande.id.y)];
					SId *idDest;
					SId *idOther;
					
					switch(commande.type) {
						case CCoup::etcHaut:
							idOther = &plateau[getIdx(commande.id.x, commande.id.y - 1)];
							idDest = &plateau[getIdx(commande.id.x, commande.id.y - 2)];
							
							ss << "\t{ property: 'cy', value: '" << ((commande.id.y - 3) * CASE_HEIGHT + MARGIN_HEIGHT);
							ss << "px', id: '#circle" << id->x << id->y << "', duration: 1000 }," << std::endl;
							
							break;
						case CCoup::etcDroite:
							idOther = &plateau[getIdx(commande.id.x + 1, commande.id.y)];
							idDest = &plateau[getIdx(commande.id.x + 2, commande.id.y)];
							
							ss << "\t{ property: 'cx', value: '" << ((commande.id.x - 'A' + 2) * CASE_WIDTH + MARGIN_WIDTH);
							ss << "px', id: '#circle" << id->x << id->y << "', duration: 1000 }," << std::endl;
							
							break;
						case CCoup::etcBas:
							idOther = &plateau[getIdx(commande.id.x, commande.id.y + 1)];
							idDest = &plateau[getIdx(commande.id.x, commande.id.y + 2)];
							
							ss << "\t{ property: 'cy', value: '" << ((commande.id.y + 1) * CASE_HEIGHT + MARGIN_HEIGHT);
							ss << "px', id: '#circle" << id->x << id->y << "', duration: 1000 }," << std::endl;
							
							break;
						case CCoup::etcGauche:
							idOther = &plateau[getIdx(commande.id.x - 1, commande.id.y)];
							idDest = &plateau[getIdx(commande.id.x - 2, commande.id.y)];
							
							ss << "\t{ property: 'cx', value: '" << ((commande.id.x - 'A' - 2) * CASE_WIDTH + MARGIN_WIDTH);
							ss << "px', id: '#circle" << id->x << id->y << "', duration: 1000 }," << std::endl;
							
							break;
					}
					
					ss << "\t{ property: 'opacity', value: '0', id: '#circle" << idOther->x << idOther->y << "', duration: 200 }," << std::endl;
					
					idOther->x = 0;
					idOther->y = 0;
					
					idDest->x = id->x;
					idDest->y = id->y;
					
					id->x = 0;
					id->y = 0;
						
					result.push_back(ss.str());
				}
			}
		}
		
		std::cout << "var seq = [" << std::endl;
		for(it=result.begin();it!=result.end();it++) {
			std::cout << *it;
		}
		std::cout << "];" << std::endl;
		
		fclose(fr);
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

