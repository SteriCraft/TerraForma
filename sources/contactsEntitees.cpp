/*
            ### - PROJET GAME / contactsEntitees.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include <vector>
#include <math.h>
#include "mobs.h"

bool contactPersoTroll(std::vector<Troll*> const& listeTrolls, int posPersoX, int posPersoY)
{
    int posMobX(0), posMobY(0);
    double distance(0);
    bool contact(false);

    for (int x(listeTrolls.size()); x > 0; x--)
    {
        listeTrolls[x - 1]->recupererPosition(&posMobX, &posMobY);

        distance = sqrt(((posMobX - posPersoX) * (posMobX - posPersoX)) + ((posMobY - posPersoY) * (posMobY - posPersoY)));

        if (distance < 2 * TAILLE_BLOCK)
        {
            contact = true;
        }
        else
        {
            contact = false;
        }
    }

    return contact;
}
