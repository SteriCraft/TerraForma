#ifndef DEF_GESTION_MAP_H
#define DEF_GESTION_MAP_H

#include <SDL_ttf.h>
#include "environnement.h"

void creerMap(SDL_Surface *ecran, int numeroMap, int modeJeu, int largeurFenetre, int hauteurFenetre, TTF_Font *policeTexte);
void chargerMap(SDL_Surface *ecran, int numeroMap, int modeJeu, int largeurFenetre, int hauteurFenetre, TTF_Font *policeTexte);
void enregistrerMap(Portion_Map world[][PROFONDEUR_MONDE], int numeroMap);

#endif
