/*
          ### - PROJET GAME / environnement.cpp - ###

               Auteur: SteriCraft
      Date du fichier: 15/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <math.h>

#include "environnement.h"
#include "interface.h"

static Lumiere sourcesLumiere[LARGEUR_MONDE * LARGEUR_PARTIE_MAP][PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP];

static SDL_Surface *surfTerre(IMG_Load("textures/blocs/terre.png"));
static SDL_Surface *surfHerbe(IMG_Load("textures/blocs/herbe.png"));
static SDL_Surface *surfPierre(IMG_Load("textures/blocs/pierre.png"));
static SDL_Surface *surfCharbon(IMG_Load("textures/blocs/charbon.png"));
static SDL_Surface *surfFer(IMG_Load("textures/blocs/fer.png"));
static SDL_Surface *surfBois(IMG_Load("textures/blocs/bois.png"));
static SDL_Surface *surfBoisNaturel(IMG_Load("textures/blocs/bois_naturel.png"));
static SDL_Surface *surfFeuille(IMG_Load("textures/blocs/feuille.png"));
static SDL_Surface *casseUn(IMG_Load("textures/items/casseUn.png"));
static SDL_Surface *casseDeux(IMG_Load("textures/items/casseDeux.png"));
static SDL_Surface *casseTrois(IMG_Load("textures/items/casseTrois.png"));
static SDL_Surface *casseQuatre(IMG_Load("textures/items/casseQuatre.png"));

void initialisationEnvironnement(SDL_Surface *ecran, Portion_Map world[][PROFONDEUR_MONDE])
{
    int courbe(PROFONDEUR_PARTIE_MAP / 2); // D�part de la courbe au chunk (0;1) � la moiti� de la hauteur de ce dernier
    Portion_Map chunk[LARGEUR_MONDE][PROFONDEUR_MONDE]; // Tableau interm�diaire des chunks (sera renvoy� dans le tableau principal de controlesCamera.cpp)

    for (int x(0); x < LARGEUR_MONDE; x++) // Initialisation de la position des chunks
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
                chunk[x][y].posX = x * (TAILLE_BLOCK * LARGEUR_PARTIE_MAP);
                chunk[x][y].posY = y * (TAILLE_BLOCK * PROFONDEUR_PARTIE_MAP);
        }
    }

    for (int x(0); x < LARGEUR_MONDE; x++) // Initialisation des blocs
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    chunk[x][y].blocs[a][b].positionBloc.x = (a + (x * 32)) * TAILLE_BLOCK; // Position...
                    chunk[x][y].blocs[a][b].positionBloc.y = (b + (y * 32)) * TAILLE_BLOCK;

                    chunk[x][y].blocs[a][b].type = 0; // Type de bloc

                    chunk[x][y].blocs[a][b].timer = 0;

                    chunk[x][y].blocs[a][b].casse = 0;
                    chunk[x][y].blocs[a][b].casseMax = 0;

                    chunk[x][y].blocs[a][b].luminosite = 0;
                    chunk[x][y].blocs[a][b].sourceLumiere = false;
                }
            }
        }
    }

    for (int x(0); x < LARGEUR_MONDE * LARGEUR_PARTIE_MAP; x++)
    {
        for(int y(0); y < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP; y++)
        {
            sourcesLumiere[x][y].puissanceLumiere = 0;
        }
    }

    for (int a(0); a < LARGEUR_MONDE; a++)
    {
        for (int x(0); x < LARGEUR_PARTIE_MAP; x++)
        {
            for (int y(0); y < PROFONDEUR_PARTIE_MAP; y++)
            {
                chunk[a][0].blocs[x][y].type = AIR;
            }
        }
    }

    for (int x(0); x < LARGEUR_MONDE; x++) // G�n�ration des chunks
    {
        chunk[x][1] = generationSolAleaChunk(ecran, chunk[x][1], &courbe); // G�n�ration s�par�e pour le premier chunk (axe des abscisses -> 0)

        for (int y(2); y < PROFONDEUR_MONDE; y++)
        {
            chunk[x][y] = generationSousSolAleaChunk(ecran, chunk[x][y]); // Sous-sols
        }
    }

    generationCavernes(chunk);

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        chunk[x][1] = generationArbres(chunk[x][1]); // G�n�ration des arbres
    }

    for (int x(0); x < LARGEUR_MONDE; x++) // Enregistrement des changements dans le tableau principal de controlesCamera.cpp
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            world[x][y] = chunk[x][y];
        }
    }
}

Portion_Map generationSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk, int *courbe)
{
    int nombreAlea(0), b(0); // Variables de travail

    for (int x(0); x < LARGEUR_PARTIE_MAP; x++) // Boucle de g�n�ration al�atoire du terrain, via un algorithme � courbe al�atoire s'inscrivant dans le tableau des blocs
    {
        nombreAlea = rand()%90; // Un nombre al�atoire est g�n�r� dans l'intervalle [0;90]

        if ((nombreAlea > 80 && nombreAlea <= 90) && (*courbe) > 8) // Si le nombre obtenu est compris entre 60 (n-inclus) et 90 (inclus), alors la courbe descend d'une case
        {
            (*courbe)--;
            chunk.blocs[x][(*courbe)].type = HERBE;
            x++;
            chunk.blocs[x][(*courbe)].type = HERBE;
        }
        else if ((nombreAlea > 70 && nombreAlea <= 80) && (*courbe) < PROFONDEUR_PARTIE_MAP-8) // Si le nombre obtenu est compris entre 30 (n-inclus) et 60 (inclus), alors la courbe grimpe d'une case
        {
            (*courbe)++;
            chunk.blocs[x][(*courbe)].type = HERBE;
            x++;
            chunk.blocs[x][(*courbe)].type = HERBE;
        }
        else if ((nombreAlea > 10 && nombreAlea <= 90) && (*courbe) > 2 && (*courbe) < 8) // Augmentation de la probabilit� que la courbe descende si elle se trouve pr�s du haut du premier chunk
        {
            (*courbe)--;
            chunk.blocs[x][(*courbe)].type = HERBE;
            x++;
            chunk.blocs[x][(*courbe)].type = HERBE;
        }
        else if ((nombreAlea > 10 && nombreAlea <= 90) && (*courbe) < PROFONDEUR_PARTIE_MAP-2 && (*courbe) > PROFONDEUR_PARTIE_MAP-8) // Augmentation de la probabilit� que la courbe monte si elle se trouve pr�s du bas du premier chunk
        {
            (*courbe)++;
            chunk.blocs[x][(*courbe)].type = HERBE;
            x++;
            chunk.blocs[x][(*courbe)].type = HERBE;
        }
        else // Sinon, elle continue tout droit d'une case
        {
            chunk.blocs[x][(*courbe)].type = HERBE;
            x++;
            chunk.blocs[x][(*courbe)].type = HERBE;
        }
    }

    for (int x(0); x < LARGEUR_PARTIE_MAP; x++) // Boucle de remplissage des blocs sous la courbe (terre et pierre mais pas de minerais)
    {
        for (int y(0); y < PROFONDEUR_PARTIE_MAP; y++)
        {
            if(chunk.blocs[x][y].type == HERBE)
            {
                b = y+10;

                while (y < b && y < PROFONDEUR_PARTIE_MAP-1)
                {
                    y++;
                    chunk.blocs[x][y].type = TERRE;
                }

                while (y < PROFONDEUR_PARTIE_MAP)
                {
                    chunk.blocs[x][y].type = PIERRE;
                    y++;
                }
                break;
            }
        }
    }

    return chunk;
}

Portion_Map generationSousSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk)
{
    for (int x(0); x < LARGEUR_PARTIE_MAP; x++) // Boucle de remplissage de pierre
    {
        for (int y(0); y < PROFONDEUR_PARTIE_MAP; y++)
        {
            chunk.blocs[x][y].type = PIERRE;
        }
    }

    chunk = mineraisAleatoiresChunk(chunk); // G�n�ration des minerais

    return chunk;
}

Portion_Map mineraisAleatoiresChunk(Portion_Map chunk)
{
    int alea(0), nombreMinerais(0);

    for (int x(5); x < LARGEUR_PARTIE_MAP-5; x++) // Scan du chunk avec probabilit� d'apparition de minerais
    {
        for (int y(5); y < PROFONDEUR_PARTIE_MAP-5; y++)
        {
            alea = rand() % 100000;

            if (alea < 200 && alea > 160)
            {
                chunk.blocs[x][y].type = CHARBON;

                nombreMinerais = rand() % 12;
                nombreMinerais += 8;

                if (chunk.blocs[x + 1][y].type == PIERRE && nombreMinerais > 0) // Positionnement semi-al�atoire des minerais (bas� sur un mod�le pr�existant)
                {
                    chunk.blocs[x + 1][y].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x][y + 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x][y + 1].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 1][y + 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y + 1].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x - 2][y + 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x - 2][y + 1].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x - 1][y + 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x - 1][y + 1].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x - 1][y + 2].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x - 1][y + 2].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x][y + 2].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x][y + 2].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 1][y + 2].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y + 2].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 2][y + 2].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 2][y + 2].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x][y + 3].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x][y + 3].type = CHARBON;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 1][y + 3].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y + 3].type = CHARBON;
                    nombreMinerais--;
                }
            }

            if (alea < 160 && alea > 150)
            {
                chunk.blocs[x][y].type = FER;

                nombreMinerais = rand() % 12;
                nombreMinerais += 8;

                if (chunk.blocs[x + 1][y].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 1][y + 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y + 1].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 1][y - 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 1][y - 1].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 2][y].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 2][y].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 2][y - 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 2][y - 1].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 2][y - 2].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x + 2][y - 2].type = FER;
                    nombreMinerais--;
                }

                if (chunk.blocs[x + 3][y - 1].type == PIERRE && nombreMinerais > 0)
                {
                    chunk.blocs[x +3][y - 1].type = FER;
                    nombreMinerais--;
                }
            }
        }
    }

    return chunk;
}

void bliterEcran(SDL_Surface *ecran, Portion_Map chunk[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre)
{
    int memX(0), memY(0);

    SDL_Surface *caseLumineuse(0);

    caseLumineuse = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOCK, TAILLE_BLOCK, 32, 0, 0, 0, 0);
    SDL_FillRect(caseLumineuse, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    for (int x(0); x < LARGEUR_MONDE; x++) // Affichage de la g�n�ration � l'�cran
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    memX = chunk[x][y].blocs[a][b].positionBloc.x;
                    memY = chunk[x][y].blocs[a][b].positionBloc.y;
                    chunk[x][y].blocs[a][b].positionBloc.x -= camera.posCamX;
                    chunk[x][y].blocs[a][b].positionBloc.y -= camera.posCamY;

                    if ((memX > camera.posCamX - (2 * TAILLE_BLOCK)) && (memX < (camera.posCamX + largeurFenetre) + (2 * TAILLE_BLOCK)))
                    {
                        if ((memY > camera.posCamY - (2 * TAILLE_BLOCK)) && (memY < (camera.posCamY + hauteurFenetre) + (2 * TAILLE_BLOCK)))
                        {
                            if (chunk[x][y].blocs[a][b].type == TERRE)
                                SDL_BlitSurface(surfTerre, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == HERBE)
                                SDL_BlitSurface(surfHerbe, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == PIERRE)
                                SDL_BlitSurface(surfPierre, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == CHARBON)
                                SDL_BlitSurface(surfCharbon, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == FER)
                                SDL_BlitSurface(surfFer, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == BOIS)
                                SDL_BlitSurface(surfBois, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == BOIS_NATUREL)
                                SDL_BlitSurface(surfBoisNaturel, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            else if (chunk[x][y].blocs[a][b].type == FEUILLE)
                                SDL_BlitSurface(surfFeuille, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);

                            if (chunk[x][y].blocs[a][b].casse == 0)
                            {}
                            else if (chunk[x][y].blocs[a][b].casse < chunk[x][y].blocs[a][b].casseMax / 4)
                            {
                                SDL_BlitSurface(casseUn, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                            }
                            else if (chunk[x][y].blocs[a][b].casse < (chunk[x][y].blocs[a][b].casseMax / 4) * 3)
                            {
                                SDL_BlitSurface(casseDeux, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                            }
                            else if (chunk[x][y].blocs[a][b].casse < (chunk[x][y].blocs[a][b].casseMax / 4) * 2)
                            {
                                SDL_BlitSurface(casseTrois, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                            }
                            else if (chunk[x][y].blocs[a][b].casse <= chunk[x][y].blocs[a][b].casseMax)
                            {
                                SDL_BlitSurface(casseQuatre, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                            }

                            if (chunk[x][y].blocs[a][b].type != AIR)
                            {
                                if (chunk[x][y].blocs[a][b].luminosite < 0)
                                {
                                    chunk[x][y].blocs[a][b].luminosite = 0;
                                }
                                else if (chunk[x][y].blocs[a][b].luminosite > 15)
                                {
                                    chunk[x][y].blocs[a][b].luminosite = 15;
                                }

                                switch (chunk[x][y].blocs[a][b].luminosite)
                                {
                                    case 0:
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 1:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 238);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 2:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 221);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 3:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 204);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 4:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 187);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 5:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 170);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 6:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 153);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 7:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 136);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 8:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 119);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 9:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 102);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 10:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 85);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 11:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 68);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 12:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 51);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 13:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 34);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;

                                    case 14:
                                        SDL_SetAlpha(caseLumineuse, SDL_SRCALPHA, 17);
                                        SDL_BlitSurface(caseLumineuse, NULL, ecran, &chunk[x][y].blocs[a][b].positionBloc);
                                        break;
                                }
                            }

                        }
                    }

                    chunk[x][y].blocs[a][b].positionBloc.x = memX;
                    chunk[x][y].blocs[a][b].positionBloc.y = memY;
                }
            }
        }
    }

    SDL_FreeSurface(caseLumineuse);
}

void modifierBloc(Portion_Map chunk[][PROFONDEUR_MONDE], int typeBloc, int posX, int posY, bool casser, InterfaceJeu *interface, bool tester) // Fonction de modification de bloc (via controlesCamera.cpp)
{
    int memX(0), memY(0);
    bool ok(false);

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    memX = chunk[x][y].posX + (a * TAILLE_BLOCK);
                    memY = chunk[x][y].posY + (b * TAILLE_BLOCK);

                    if (chunk[x][y].blocs[a][b].type == AIR) // Mise en place de la casse maximale en fonction du bloc
                    {
                        chunk[x][y].blocs[a][b].casseMax = 0;
                    }
                    else if (chunk[x][y].blocs[a][b].type == HERBE)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_HERBE;
                    }
                    else if (chunk[x][y].blocs[a][b].type == PIERRE)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_PIERRE;
                    }
                    else if (chunk[x][y].blocs[a][b].type == TERRE)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_TERRE;
                    }
                    else if (chunk[x][y].blocs[a][b].type == CHARBON)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_CHARBON;
                    }
                    else if (chunk[x][y].blocs[a][b].type == FER)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_FER;
                    }
                    else if (chunk[x][y].blocs[a][b].type == BOIS)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_BOIS;
                    }
                    else if (chunk[x][y].blocs[a][b].type == BOIS_NATUREL)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_BOIS_NATUREL;
                    }
                    else if (chunk[x][y].blocs[a][b].type == FEUILLE)
                    {
                        chunk[x][y].blocs[a][b].casseMax = CASSE_FEUILLE;
                    }

                    if (posX > memX && (posX < memX + TAILLE_BLOCK) && posY > memY && (posY < memY + TAILLE_BLOCK))
                    {
                        if (chunk[x][y].blocs[a][b].type == AIR || casser)
                        {
                            if (casser)
                            {
                                if (tester) // Si on test, c'est que l'on est en mode aventure
                                {
                                    if (chunk[x][y].blocs[a][b].casse >= chunk[x][y].blocs[a][b].casseMax)
                                    {
                                        interface->ajouterEnleverBlocInventaire(chunk[x][y].blocs[a][b].type, true, &ok);
                                        chunk[x][y].blocs[a][b].type = typeBloc;
                                        chunk[x][y].blocs[a][b].casse = 0;
                                    }
                                    else
                                    {
                                        chunk[x][y].blocs[a][b].casse++;
                                    }
                                }
                                else
                                {
                                    chunk[x][y].blocs[a][b].type = typeBloc;
                                }
                            }
                            else
                            {
                                if (tester)
                                {
                                    interface->ajouterEnleverBlocInventaire(typeBloc, false, &ok);
                                    if (ok)
                                    {
                                        chunk[x][y].blocs[a][b].type = typeBloc;
                                    }
                                }
                                else
                                {
                                    chunk[x][y].blocs[a][b].type = typeBloc;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

Portion_Map generationArbres(Portion_Map chunk)
{
    int nombreAlea(0);
    bool end(false);

    for (int x(5); x < LARGEUR_PARTIE_MAP - 5; x++)
    {
        for (int y(7); y < LARGEUR_PARTIE_MAP - 7; y++)
        {
            nombreAlea = rand() % 50;

            if (nombreAlea < 5 && (chunk.blocs[x][y].type == HERBE || chunk.blocs[x][y].type == TERRE) && chunk.blocs[x][y - 1].type == AIR)
            {
                chunk.blocs[x][y].type = TERRE;
                chunk.blocs[x][y - 1].type = BOIS_NATUREL;
                chunk.blocs[x][y - 2].type = BOIS_NATUREL;
                chunk.blocs[x][y - 3].type = BOIS_NATUREL;
                chunk.blocs[x][y - 4].type = BOIS_NATUREL;
                chunk.blocs[x - 1][y - 4].type = FEUILLE;
                chunk.blocs[x - 2][y - 4].type = FEUILLE;
                chunk.blocs[x + 1][y - 4].type = FEUILLE;
                chunk.blocs[x + 2][y - 4].type = FEUILLE;
                chunk.blocs[x - 1][y - 5].type = FEUILLE;
                chunk.blocs[x - 2][y - 5].type = FEUILLE;
                chunk.blocs[x + 1][y - 5].type = FEUILLE;
                chunk.blocs[x + 2][y - 5].type = FEUILLE;
                chunk.blocs[x][y - 5].type = FEUILLE;
                chunk.blocs[x - 1][y - 6].type = FEUILLE;
                chunk.blocs[x - 2][y - 6].type = FEUILLE;
                chunk.blocs[x + 1][y - 6].type = FEUILLE;
                chunk.blocs[x + 2][y - 6].type = FEUILLE;
                chunk.blocs[x][y - 6].type = FEUILLE;
                chunk.blocs[x + 1][y - 7].type = FEUILLE;
                chunk.blocs[x - 1][y - 7].type = FEUILLE;
                chunk.blocs[x][y - 7].type = FEUILLE;

                if (x + 5 < LARGEUR_PARTIE_MAP - 5)
                {
                    y = 7;
                    x += 5;
                }
                else
                {
                    end = true;
                    break;
                }
            }
        }

        if (end)
        {
            break;
        }
    }

    return chunk;
}

void generationCavernes(Portion_Map world[][PROFONDEUR_MONDE])
{
    int chunk[LARGEUR_MONDE * LARGEUR_PARTIE_MAP][PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP], courbeX(0), courbeY(0), nombreCavernes(0), longueurCaverne(0), direction(0), epaisseur(0), mem(0), sens(0);

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)] = world[x][y].blocs[a][b].type;
                }
            }
        }
    }

    nombreCavernes = rand() % 4;
    nombreCavernes += 2;

    while (nombreCavernes > 0)
    {
        courbeX = rand() % (LARGEUR_MONDE * LARGEUR_PARTIE_MAP);
        courbeY = rand() % (PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP);

        longueurCaverne = rand() % 80;
        longueurCaverne += 50;

        epaisseur = rand() % 3;
        epaisseur += 3;

        sens = rand() % 2;

        if (sens == 0)
        {
            while (longueurCaverne > 0)
            {
                chunk[courbeX][courbeY] = AIR;

                mem = epaisseur / 2;

                if ((courbeY + mem) < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP && (courbeY - mem > 0))
                {
                    while (mem > 0)
                    {

                        chunk[courbeX][courbeY + mem] = AIR;
                        chunk[courbeX][courbeY - mem] = AIR;
                        mem--;
                    }
                }

                mem = rand() % 3;

                if (mem == 0 && epaisseur + 1 < 10)
                {
                    epaisseur++;
                }
                else if (mem == 1 && epaisseur - 1 > 3)
                {
                    epaisseur--;
                }

                direction = rand() % 11;

                if (direction <= 4 && courbeX + 1 < LARGEUR_MONDE * LARGEUR_PARTIE_MAP)
                {
                    courbeX++;
                }
                else if (direction <= 9 && direction > 4 && courbeX + 1 < LARGEUR_MONDE * LARGEUR_PARTIE_MAP && courbeY + 1 < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP)
                {
                    courbeX++;
                    courbeY++;
                }
                else if (direction == 10 && (courbeX + 1)  < LARGEUR_MONDE * LARGEUR_PARTIE_MAP && (courbeY - 1 > 0))
                {
                    courbeX++;
                    courbeY--;
                }

                longueurCaverne--;
            }
        }
        else
        {
            while (longueurCaverne > 0)
            {
                chunk[courbeX][courbeY] = AIR;

                mem = epaisseur / 2;

                while (mem > 0)
                {
                    if (courbeY + mem < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP && courbeY - mem > 0)
                    {
                        chunk[courbeX][courbeY + mem] = AIR;
                        chunk[courbeX][courbeY - mem] = AIR;
                    }

                    mem--;
                }

                mem = rand() % 3;

                if (mem == 0 && epaisseur + 1 < 10)
                {
                    epaisseur++;
                }
                else if (mem == 1 && epaisseur - 1 > 3)
                {
                    epaisseur--;
                }

                direction = rand() % 11;

                if (direction <= 4 && courbeX - 1 > 0)
                {
                    courbeX--;
                }
                else if (direction <= 9 && direction > 4 && courbeX - 1 > 0 * LARGEUR_PARTIE_MAP && courbeY + 1 < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP)
                {
                    courbeX--;
                    courbeY++;
                }
                else if (direction == 10 && courbeX - 1  > 0 * LARGEUR_PARTIE_MAP && courbeY - 1 > 0)
                {
                    courbeX--;
                    courbeY--;
                }

                longueurCaverne--;
            }
        }

        nombreCavernes--;
    }

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    world[x][y].blocs[a][b].type = chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)];
                }
            }
        }
    }
}

void bliterArrierePlan(SDL_Surface *ecran, Camera camera, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE], int tempsJour)
{
    int chunkX(0), chunkY(0), blocX(0), blocY(0);

    SDL_Rect positionBloc;
    SDL_Rect positionSoleil;
    SDL_Surface *soleil(IMG_Load("textures/ciel/soleil.png"));
    SDL_Surface *lune(IMG_Load("textures/ciel/lune.png"));
    SDL_Surface *pierreBackGround(IMG_Load("textures/ciel/backGroundPierre.png"));
    SDL_Surface *terreBackGround(IMG_Load("textures/ciel/backGroundTerre.png"));

    positionSoleil.x = largeurFenetre / 4;
    positionSoleil.y = hauteurFenetre / 4;

    if (tempsJour >= 32400 && tempsJour <= 75600)
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 119, 181, 254));
        SDL_BlitSurface(soleil, NULL, ecran, &positionSoleil);

        for (int x(0); x < LARGEUR_MONDE; x++)
        {
            for (int y(0); y < PROFONDEUR_MONDE; y++)
            {
                for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
                {
                    for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                    {
                        world[x][y].blocs[a][b].luminosite = 15;
                    }
                }
            }
        }
    }
    else
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 4, 7, 10));
        SDL_BlitSurface(lune, NULL, ecran, &positionSoleil);

        for (int x(0); x < LARGEUR_MONDE; x++)
        {
            for (int y(0); y < PROFONDEUR_MONDE; y++)
            {
                for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
                {
                    for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                    {
                        world[x][y].blocs[a][b].luminosite = 3;
                    }
                }
            }
        }
    }

    for (int x(0); x < LARGEUR_MONDE * LARGEUR_PARTIE_MAP * TAILLE_BLOCK; x += TAILLE_BLOCK)
    {
        for (int y(0); y < LARGEUR_MONDE * LARGEUR_PARTIE_MAP * TAILLE_BLOCK; y += TAILLE_BLOCK)
        {
            positionBloc.x = x - camera.posCamX;
            positionBloc.y = y - camera.posCamY;

            chunkX = (x / TAILLE_BLOCK) / LARGEUR_PARTIE_MAP;
            chunkY = (y / TAILLE_BLOCK) / PROFONDEUR_PARTIE_MAP;
            blocX = (x / TAILLE_BLOCK) - (chunkX * LARGEUR_PARTIE_MAP);
            blocY = (y / TAILLE_BLOCK) - (chunkY * PROFONDEUR_PARTIE_MAP);

            if ((chunkY * PROFONDEUR_PARTIE_MAP) + blocY > 54 && (chunkY * PROFONDEUR_PARTIE_MAP) + blocY <= 64 && world[chunkX][chunkY].blocs[blocX][blocY].type == AIR)
            {
                if (x > camera.posCamX - (2 * TAILLE_BLOCK) && x < (camera.posCamX + largeurFenetre) + (2 * TAILLE_BLOCK))
                {
                    if (y > camera.posCamY - (2 * TAILLE_BLOCK) && y < (camera.posCamY + hauteurFenetre) + (2 * TAILLE_BLOCK))
                    {
                        SDL_BlitSurface(terreBackGround, NULL, ecran, &positionBloc);
                    }
                }
            }
            else if ((chunkY * PROFONDEUR_PARTIE_MAP) + blocY > 64 && world[chunkX][chunkY].blocs[blocX][blocY].type == AIR)
            {
                SDL_BlitSurface(pierreBackGround, NULL, ecran, &positionBloc);
            }
        }
    }

    SDL_FreeSurface(soleil);
    SDL_FreeSurface(lune);
    SDL_FreeSurface(pierreBackGround);
    SDL_FreeSurface(terreBackGround);
}

void majTerre(Portion_Map world[][PROFONDEUR_MONDE])
{
    for (int a(0); a < LARGEUR_MONDE; a++)
    {
        for (int b(0); b < PROFONDEUR_MONDE; b++)
        {
            for (int x(0); x < LARGEUR_PARTIE_MAP; x++)
            {
                for (int y(1); y < PROFONDEUR_PARTIE_MAP - 1; y++)
                {
                    if (world[a][b].blocs[x][y].type == TERRE)
                    {
                        if (world[a][b].blocs[x][y - 1].type == AIR)
                        {
                            if (world[a][b].blocs[x][y].timer == 0)
                            {
                                world[a][b].blocs[x][y].timer = (rand() % 50) + 50;
                            }
                            else if (world[a][b].blocs[x][y].timer == 10)
                            {
                                world[a][b].blocs[x][y].type = HERBE;
                                world[a][b].blocs[x][y].timer = 0;
                            }
                            else
                            {
                                world[a][b].blocs[x][y].timer -= 1;
                            }
                        }
                    }
                                        if (world[a][b].blocs[x][y].type == HERBE)
                    {
                        if (world[a][b].blocs[x][y - 1].type != AIR)
                        {
                            if (world[a][b].blocs[x][y].timer == 0)
                            {
                                world[a][b].blocs[x][y].timer = (rand() % 50) + 50;
                            }
                            else if (world[a][b].blocs[x][y].timer == 10)
                            {
                                world[a][b].blocs[x][y].type = TERRE;
                                world[a][b].blocs[x][y].timer = 0;
                            }
                            else
                            {
                                world[a][b].blocs[x][y].timer -= 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void suppressionSurface()
{
    SDL_FreeSurface(surfTerre);
    SDL_FreeSurface(surfHerbe);
    SDL_FreeSurface(surfPierre);
    SDL_FreeSurface(surfCharbon);
    SDL_FreeSurface(surfFer);
    SDL_FreeSurface(surfBois);
    SDL_FreeSurface(surfBoisNaturel);
    SDL_FreeSurface(surfFeuille);
    SDL_FreeSurface(casseUn);
    SDL_FreeSurface(casseDeux);
    SDL_FreeSurface(casseTrois);
    SDL_FreeSurface(casseQuatre);
}

void creerSourceLumiere(Portion_Map world[][PROFONDEUR_MONDE], int posX, int posY, int puissance)
{
    int chunkX(0), chunkY(0);

    sourcesLumiere[posX][posY].puissanceLumiere = puissance;

    chunkX = posX / LARGEUR_PARTIE_MAP;
    posX -= chunkX * LARGEUR_PARTIE_MAP;
    chunkY = posY / LARGEUR_PARTIE_MAP;
    posY -= chunkY * LARGEUR_PARTIE_MAP;

    world[chunkX][chunkY].blocs[posX][posY].sourceLumiere = true;
}

void appliquerLumiere(Portion_Map world[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre)
{
    int distance(0), chunkType[LARGEUR_MONDE * LARGEUR_PARTIE_MAP][PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP], somme(0);
    TestSourceLumiere chunk[LARGEUR_MONDE * LARGEUR_PARTIE_MAP][PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP];

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    chunkType[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)] = world[x][y].blocs[a][b].type;
                    chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)].luminosite = world[x][y].blocs[a][b].luminosite;
                    chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)].active = world[x][y].blocs[a][b].sourceLumiere;
                }
            }
        }
    }

    for (int x(3); x < LARGEUR_MONDE * LARGEUR_PARTIE_MAP - 3; x++)
    {
        for (int y(3); y < PROFONDEUR_MONDE * PROFONDEUR_PARTIE_MAP - 3; y++)
        {
            if ((x * TAILLE_BLOCK > camera.posCamX - (2 * TAILLE_BLOCK)) && (x * TAILLE_BLOCK < (camera.posCamX + largeurFenetre) + (2 * TAILLE_BLOCK)))
            {
                if ((y * TAILLE_BLOCK > camera.posCamY - (2 * TAILLE_BLOCK)) && (y * TAILLE_BLOCK < (camera.posCamY + hauteurFenetre) + (2 * TAILLE_BLOCK)))
                {
                    if (chunkType[x][y] != AIR && chunkType[x][y] != FEUILLE)
                    {
                        if (!chunkType[x][y] == AIR)
                        {
                            somme = 0;

                            for (int a(x - 3); a < x + 3; a++)
                            {
                                for (int b(y - 3); b < y + 3; b++)
                                {
                                    distance = sqrt((a - x) * (a - x) + (b - y) * (b - y));

                                    if (distance < 3)
                                    {
                                        if (chunkType[a][b] != AIR)
                                        {
                                            somme++;
                                        }
                                    }
                                }
                            }

                            if (somme == 25)
                            {
                                chunk[x][y].luminosite = 0;
                            }
                        }
                    }

                    /*if (chunk[x][y].active) // Gestion des sources de lumi�res, � finir
                    {
                        for (int a(x - 15); a < x + 15; a++)
                        {
                            for (int b(y - 15); b < y + 15; b++)
                            {
                                distance = sqrt((a - x) * (a - x) + (b - y) * (b - y));

                                if (distance < 15 && distance > 0.5)
                                {
                                    if (chunk[a][b].luminosite + (sourcesLumiere[x][y].puissanceLumiere - log(distance)) > chunk[a][b].luminosite)
                                    {
                                        chunk[a][b].luminosite += sourcesLumiere[x][y].puissanceLumiere - log(distance);
                                    }
                                }
                            }
                        }

                        chunk[x][y].luminosite += sourcesLumiere[x][y].puissanceLumiere;
                    }*/
                }
            }
        }
    }

    for (int x(0); x < LARGEUR_MONDE; x++)
    {
        for (int y(0); y < PROFONDEUR_MONDE; y++)
        {
            for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
            {
                for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
                {
                    world[x][y].blocs[a][b].luminosite = chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)].luminosite;
                    world[x][y].blocs[a][b].sourceLumiere = chunk[a + (x * LARGEUR_PARTIE_MAP)][b + (y * PROFONDEUR_PARTIE_MAP)].active;
                }
            }
        }
    }
}
