/*
           ### - PROJET GAME / environnement.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#ifndef DEF_ENVIRONNEMENT_H
#define DEF_ENVIRONNEMENT_H

#include <SDL/SDL.h>
#include "interface.h"

#define LARGEUR_MONDE 5 // Paramètres de la map
#define PROFONDEUR_MONDE 5
#define LARGEUR_PARTIE_MAP 32
#define PROFONDEUR_PARTIE_MAP 32
#define TAILLE_BLOCK 16

#define AIR 0 // Numération des blocs
#define HERBE 1
#define TERRE 2
#define PIERRE 3
#define CHARBON 4
#define FER 5
#define BOIS 6
#define BOIS_NATUREL 7
#define FEUILLE 8

#define MAX_HERBE 120 // Quantité maximale de blocs dans un stack, par type
#define MAX_TERRE 100
#define MAX_PIERRE 70
#define MAX_CHARBON 80
#define MAX_FER 60
#define MAX_BOIS 80
#define MAX_BOIS_NATUREL 60
#define MAX_FEUILLE 200

typedef struct // Structure de base d'un bloc
{
    SDL_Rect positionBloc;
    int type;
    int coorX;
    int coorY;
    int timer;
    int casse;
} BlocBrut;

typedef struct // Structure de base d'un chunk
{
    BlocBrut blocs[LARGEUR_PARTIE_MAP][PROFONDEUR_PARTIE_MAP];
    int posX;
    int posY;
} Portion_Map;

typedef struct Camera // Structure de base de la caméra
{
    double posCamX;
    double posCamY;
} Camera;

void initialisationEnvironnement(SDL_Surface *ecran, Portion_Map world[][PROFONDEUR_MONDE]);
Portion_Map generationSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk, int *a);
Portion_Map generationSousSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk);
Portion_Map mineraisAleatoiresChunk(Portion_Map chunk);
void bliterEcran(SDL_Surface *ecran, Portion_Map chunk[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre);
void modifierBloc(Portion_Map chunk[][PROFONDEUR_MONDE], int typeBloc, int posX, int posY, bool casser, InterfaceJeu *interface);
Portion_Map generationArbres(Portion_Map chunk);
void generationCavernes(Portion_Map world[][PROFONDEUR_MONDE]);
void bliterArbres(SDL_Surface *ecran, Portion_Map chunk[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre);
void bliterArrierePlan(SDL_Surface *ecran, Camera camera, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE]);
void majTerre(Portion_Map world[][PROFONDEUR_MONDE]);

#endif
