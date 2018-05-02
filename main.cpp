#include <ncurses.h>
#include <iostream>
#include "CSolver.h"

int main(void) {
	CSolver solver;
	
	std::cout << solver.getNbFeuille() << std::endl;
	solver.process();
	
	return 0;
}
