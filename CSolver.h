#ifndef __CSOLVER__
#define __CSOLVER__

#include <functional>
#include "CPlateaux.h"
#include "CPlateau.h"

// Nombre maximal de plateaux retenus à chaque étape
#define MAX_TEST	5000

// Forme du plateau vide (UNDEF hors plateau, VIDE pour un trou)
extern unsigned char modele[NB_BILLE];

class CSolver {
public:
	// Appelé à chaque étape avec les plateaux retenus (triés par poids) et le nombre de billes
	typedef std::function<void(CPlateaux *, int)> FctVisu;
private:
	CPlateaux *plateaux;
	CPlateau *solution;
	FctVisu visu;

	void init(void);
	bool addPlateauIfNotExistst(CPlateaux *plateaux, CPlateau *plateau);
	void clearPlateaux(void);
public:
	CSolver(FctVisu visu = nullptr);
	~CSolver(void);
	int getNbPlateaux(void);
	void process(void);
	// Plateau de départ de la solution (coups dans l'ordre du jeu), 0 si aucune
	CPlateau * getSolution(void);
};

#endif //__CSOLVER__
