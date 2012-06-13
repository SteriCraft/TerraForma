/*
          ### - PROJET GAME / mobs.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include "mobs.h"

Troll::Troll(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre) : Personnage(texturePerso, world, camera, largeurFenetre, hauteurFenetre)
{
    forcePoing = FORCE_TROLL;
}
