/*
          ### - PROJET GAME / controlesCamera.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H

#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include "environnement.h"

#define LARGEUR_FENETRE 1680
#define PROFONDEUR_FENETRE 1050

typedef struct // Structure de la mise à jour clavier
{
    bool key[SDLK_LAST];
    bool quit;
    bool boutonSouris[5];
    int sourisX;
    int sourisY;
} ReceptionClavier;

void controlesCamera(SDL_Surface *ecran, int modeJeu, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE], TTF_Font *policeTexte);
bool verifModifBloc(double posPersoX, double posPersoY, double posSourisX, double posSourisY);

#endif
