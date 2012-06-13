/*
          ### - PROJET GAME / lecteurFichier.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 05/06/2012
*/

#include <iostream>
#include <fstream>
#include <string>
#include "lecteurFichier.h"

void parametresFenetre(bool *fullscreen, int *largeurFenetre, int *hauteurFenetre, bool ecritureLecture)
{
    if (ecritureLecture) // Ecriture
    {
        std::ofstream config("config/config.cfg");

        config << "FULLSCREEN = ";

        if (*fullscreen)
        {
            config << "ENABLED" << std::endl;
        }
        else
        {
            config << "DISABLED" << std::endl;
        }

        config << "LARGEUR = " << *largeurFenetre << std::endl;
        config << "HAUTEUR = " << *hauteurFenetre;
    }
    else // Lecture
    {
        std::ifstream config("config/config.cfg");

        std::string mot;

        config >> mot;
        config >> mot;
        config >> mot;

        if (mot == "ENABLED")
        {
            *fullscreen = true;
        }
        else
        {
            *fullscreen = false;

            config >> mot;
            config >> mot;
            config >> *largeurFenetre;

            config >> mot;
            config >> mot;
            config >> *hauteurFenetre;
        }
    }
}
