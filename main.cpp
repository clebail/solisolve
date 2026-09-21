#include <string.h>
#include "CSolver.h"

int main(int argc, char *argv[]) {
	bool visu = (argc > 1 && strcmp(argv[1], "-v") == 0);
	CSolver solver(visu);

	solver.process();

    return 0;
}
