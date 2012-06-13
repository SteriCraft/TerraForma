#ifndef DEF_MOBS_H
#define DEF_MOBS_H

#include "personnage.h"

#define FORCE_TROLL 30

class Troll : public Personnage
{
    public:
        Troll(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre);

    private:
        int forcePoing;
};

#endif
