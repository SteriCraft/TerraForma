/*
          ### - PROJET GAME / tableParametresBloc.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include "environnement.h"

bool testBlocSolide(int typeBloc) // Vérification de la solidité des blocs
{
    bool blocs[9] = {false, true, true, true, true, true, true, true, false};

    return blocs[typeBloc];
}
