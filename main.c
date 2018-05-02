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

	getch();
	
	endwin();
	
	while(solution != 0) {
		SCoup *coup = popCoup(&solution);
	
		printCoup(coup);
		
		free(coup);
	}
	
	return 0;
}
