/*
            ### - PROJET GAME / mobs.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#ifndef DEF_MOBS_H
#define DEF_MOBS_H

#include "personnage.h"

#define FORCE_TROLL 30 // Force de base d'un troll

class Troll : public Personnage
{
    public:
        Troll(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre);

    private:
        int forcePoing;
};

#endif
