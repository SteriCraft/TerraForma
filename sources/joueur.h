/*
          ### - PROJET GAME / joueur.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#ifndef DEF_JOUEUR_H
#define DEF_JOUEUR_H

#include "personnage.h"

class Joueur : public Personnage
{
    public:
        Joueur(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre);
        ~Joueur();

    private:
        int mana;
        int exp;
        int forceBrute;
        int forcePoing;
};

#endif
