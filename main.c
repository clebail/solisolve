#include <ncurses.h>
#include <stdlib.h>
#include "SCoup.h"
#include "TPlateau.h"

int main(void) {
	TPlateau plateau;
	SListCoup *solution;
	
	initscr();
	
	init(plateau, 3, 0);
	solution = solve(plateau);
	
	print(plateau);
	
	while(solution != 0) {
		SCoup *coup = popCoup(&solution);
	
		joue(plateau, coup);
		
		getch();
		print(plateau);
		free(coup);
	}
	
	endwin();
	
	return 0;
}