/*
Les commentaires de ce fichier ne sont pas terminés, en raison de travaux en cours sur le code
*/

#include <iostream>
#include <fstream>
#include <string>
#include <SDL_ttf.h>
#include "gestionMap.h"
#include "controlesCamera.h"

void creerMap(SDL_Surface *ecran, int numeroMap, int modeJeu, int largeurFenetre, int hauteurFenetre, TTF_Font *policeTexte)
{
    Portion_Map world[LARGEUR_MONDE][PROFONDEUR_MONDE];

    initialisationEnvironnement(ecran, world);

    enregistrerMap(world, numeroMap);

    controlesCamera(ecran, modeJeu, largeurFenetre, hauteurFenetre, world, policeTexte);
}

void chargerMap(SDL_Surface *ecran, int numeroMap, int modeJeu, int largeurFenetre, int hauteurFenetre, TTF_Font *policeTexte)
{
    Portion_Map world[LARGEUR_MONDE][PROFONDEUR_MONDE];

    std::string nomFichierMap(""), mot("");

    if (numeroMap == 1)
    {
        nomFichierMap = "saves/save1.sav";
    }
    else if (numeroMap == 2)
    {
        nomFichierMap = "saves/save2.sav";
    }
    else if (numeroMap == 3)
    {
        nomFichierMap = "saves/save3.sav";
    }
    else if (numeroMap == 4)
    {
        nomFichierMap = "saves/save4.sav";
    }

    std::ifstream fichierMap(nomFichierMap.c_str());

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    fichierMap >> mot;

                    if (mot == "001")
                    {
                        world[x][y].blocs[a][b].type = TERRE;
                    }
                    else if (mot == "002")
                    {
                        world[x][y].blocs[a][b].type = HERBE;
                    }
                    else if (mot == "003")
                    {
                        world[x][y].blocs[a][b].type = PIERRE;
                    }
                    else if (mot == "004")
                    {
                        world[x][y].blocs[a][b].type = CHARBON;
                    }
                    else if (mot == "005")
                    {
                        world[x][y].blocs[a][b].type = FER;
                    }
                    else if (mot == "006")
                    {
                        world[x][y].blocs[a][b].type = BOIS;
                    }
                    else if (mot == "007")
                    {
                        world[x][y].blocs[a][b].type = BOIS_NATUREL;
                    }
                    else if (mot == "008")
                    {
                        world[x][y].blocs[a][b].type = FEUILLE;
                    }
                    else
                    {
                        world[x][y].blocs[a][b].type = AIR;
                    }
                }
            }
        }
    }

    controlesCamera(ecran, modeJeu, largeurFenetre, hauteurFenetre, world, policeTexte);
}

void enregistrerMap(Portion_Map world[][PROFONDEUR_MONDE], int numeroMap)
{
    std::string nomFichierMap("");

    if (numeroMap == 1)
    {
        nomFichierMap = "saves/save1.sav";
    }
    else if (numeroMap == 2)
    {
        nomFichierMap = "saves/save2.sav";
    }
    else if (numeroMap == 3)
    {
        nomFichierMap = "saves/save3.sav";
    }
    else if (numeroMap == 4)
    {
        nomFichierMap = "saves/save4.sav";
    }

    std::ofstream fichierMap(nomFichierMap.c_str());

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    if (world[x][y].blocs[a][b].type == AIR)
                    {
                        fichierMap << "000 ";
                    }
                    else if (world[x][y].blocs[a][b].type == TERRE)
                    {
                        fichierMap << "001 ";
                    }
                    else if (world[x][y].blocs[a][b].type == HERBE)
                    {
                        fichierMap << "002 ";
                    }
                    else if (world[x][y].blocs[a][b].type == PIERRE)
                    {
                        fichierMap << "003 ";
                    }
                    else if (world[x][y].blocs[a][b].type == CHARBON)
                    {
                        fichierMap << "004 ";
                    }
                    else if (world[x][y].blocs[a][b].type == FER)
                    {
                        fichierMap << "005 ";
                    }
                    else if (world[x][y].blocs[a][b].type == BOIS)
                    {
                        fichierMap << "006 ";
                    }
                    else if (world[x][y].blocs[a][b].type == BOIS_NATUREL)
                    {
                        fichierMap << "007 ";
                    }
                    else if (world[x][y].blocs[a][b].type == FEUILLE)
                    {
                        fichierMap << "008 ";
                    }
                }
            }
        }
    }
}
