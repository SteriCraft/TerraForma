/*
          ### - PROJET GAME / joueur.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 05/06/2012
*/

#include "joueur.h"

Joueur::Joueur(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre) : Personnage(texturePerso, world, camera, largeurFenetre, hauteurFenetre, 100)
{
    mana = 10;
    exp = 0;
    forceBrute = 20;
    forcePoing = 10;
}

Joueur::~Joueur()
{
}
