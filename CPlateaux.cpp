#include <iostream>
#include <ncurses.h>
#include "CPlateaux.h"

void CPlateaux::getCoords(int idx, char& x, int& y) {
	x = idx % NB_COLONNE + 'A';
	y = idx / NB_COLONNE + 1;
}

CGene * CPlateaux::getGene(int idx) {
	return &plateaux[idx];
}

void CPlateaux::init(void) {
	int i;
	
	do {
		for(i = 0;i<MAX_BILLE-1;i++) {
			plateaux[i].init(i+1);
		}
		
		calculScore();
	}while(score == 0);
	
	std::cout << "Score : " << score << std::endl;
}

int CPlateaux::getScore(void) {
	return score;
}

void CPlateaux::calculScore(void) {
	int i;
	int first = false;
	int ox, oy;
	
	coups.clear();
	
	//initscr();
	
	score = 0;
	for(i=0;i<MAX_BILLE-2;i++) {
		CCoup coup;
		
		if(plateaux[i+1].isNext(&plateaux[i], coup)) {
			if(!first) {
				first = true;
				score += i;
			}
			score += i+1;
			coups.push_back(coup);
	
			//move(51, i*2);
			//printw("1");
		} else {
			first = false;
			
			//move(51, i*2);
			//printw("0");
		}
	}
	
	
	/*ox=oy=0;
	for(i=0;i<MAX_BILLE-1;i++) {
		plateaux[i].print(ox, oy);
		
		ox += 10;
		if(ox >= 100) {
			ox = 0;
			oy += 10;
		}
	}*/
	
	//move(52, 0);
	//printw("%d", score);
	
	//getch();
    //endwin();
}

void CPlateaux::mute(int idxGene) {
	plateaux[idxGene].init(plateaux[idxGene].getNbTrou());
}

void CPlateaux::from(CIndividu *i1, CIndividu *i2, int seuil) {
	CPlateaux *source = static_cast<CPlateaux *>(i1);
	int i;
	
	for(i=0;i<MAX_BILLE-1;i++) {
		if(i >= seuil) {
			source = static_cast<CPlateaux *>(i2);
		}
		
		plateaux[i].from(static_cast<CPlateau *>(source->getGene(i)));
	}
}

void CPlateaux::print(void) {
	int depart =  *(plateaux[0].getTrous().begin());
	char x;
	int y;
	std::list<CCoup>::iterator itCoup;
	
	getCoords(depart, x, y);
	std::cout << "D " << x << "," << y;
	
	for(itCoup=coups.begin();itCoup!=coups.end();itCoup++) {
		CCoup coup = *itCoup;
		
		getCoords(coup.getDepuis(), x, y);
		std::cout << " M " << x << "," << y << " " << (int)coup.getType();
	}
}
