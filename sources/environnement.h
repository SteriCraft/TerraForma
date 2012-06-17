/*
           ### - PROJET GAME / environnement.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 17/06/2012
*/

#ifndef DEF_ENVIRONNEMENT_H
#define DEF_ENVIRONNEMENT_H

#include <SDL/SDL.h>
#include "interface.h"

#define LARGEUR_MONDE 5
#define PROFONDEUR_MONDE 5
#define LARGEUR_PARTIE_MAP 32
#define PROFONDEUR_PARTIE_MAP 32
#define TAILLE_BLOCK 32

/*
Pour ajouter un nouveau bloc, mettre à jour les defines dans environnement.h
Mettre à jour bliterEcran dans environnement.cpp
Mettre à jour modifierBloc dans environnement.cpp
Mettre à jour le constructeur dans interface.cpp
Mettre à jour le destructeur dans interface.cpp
Mettre à jour chargerTexturesItems dans interface.cpp
Mettre à jour afficherInventaire dans interface.cpp
Mettre à jour ajouterEnleverBlocInventaire dans interface.cpp
Mettre à jour interface.h
*/

#define AIR 0
#define HERBE 1
#define TERRE 2
#define PIERRE 3
#define CHARBON 4
#define FER 5
#define BOIS 6
#define BOIS_NATUREL 7
#define FEUILLE 8

#define MAX_HERBE 120
#define MAX_TERRE 100
#define MAX_PIERRE 70
#define MAX_CHARBON 80
#define MAX_FER 60
#define MAX_BOIS 80
#define MAX_BOIS_NATUREL 60
#define MAX_FEUILLE 200

#define CASSE_HERBE 40
#define CASSE_TERRE 40
#define CASSE_PIERRE 300
#define CASSE_CHARBON 400
#define CASSE_FER 600
#define CASSE_BOIS 100
#define CASSE_BOIS_NATUREL 100
#define CASSE_FEUILLE 10

typedef struct
{
    SDL_Rect positionBloc;
    int type;
    int timer;
    int casse;
    int casseMax;
    int luminosite;
    bool sourceLumiere;
} BlocBrut;

typedef struct
{
    int puissanceLumiere;
} Lumiere;

typedef struct
{
    BlocBrut blocs[LARGEUR_PARTIE_MAP][PROFONDEUR_PARTIE_MAP];
    int posX;
    int posY;
} Portion_Map;

typedef struct
{
    double posCamX;
    double posCamY;
} Camera;

typedef struct
{
    int luminosite;
    bool active;
} TestSourceLumiere;

void initialisationEnvironnement(SDL_Surface *ecran, Portion_Map world[][PROFONDEUR_MONDE]);
Portion_Map generationSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk, int *a);
Portion_Map generationSousSolAleaChunk(SDL_Surface *ecran, Portion_Map chunk);
Portion_Map mineraisAleatoiresChunk(Portion_Map chunk);
void bliterEcran(SDL_Surface *ecran, Portion_Map chunk[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre);
void modifierBloc(Portion_Map chunk[][PROFONDEUR_MONDE], int typeBloc, int posX, int posY, bool casser, InterfaceJeu *interface, bool tester);
Portion_Map generationArbres(Portion_Map chunk);
void generationCavernes(Portion_Map world[][PROFONDEUR_MONDE]);
void bliterArrierePlan(SDL_Surface *ecran, Camera camera, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE], int tempsJour);
void majTerre(Portion_Map world[][PROFONDEUR_MONDE]);
void suppressionSurface();
void creerSourceLumiere(Portion_Map world[][PROFONDEUR_MONDE], int posX, int posY, int puissance);
void appliquerLumiere(Portion_Map world[][PROFONDEUR_MONDE], Camera camera, int largeurFenetre, int hauteurFenetre);

#endif
