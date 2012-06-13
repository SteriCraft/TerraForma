/*
            ### - PROJET GAME / menuPrincipal.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#ifndef DEF_MENU_PRINCIPAL_H
#define DEF_MENU_PRINCIPAL_H

#include <SDL_ttf.h>

#define AVENTURE 0
#define LIBRE 1

typedef struct
{
    SDL_Color couleurTexte; // Structure des surfaces écrites
    SDL_Surface *texte;
    SDL_Rect positionTexte;
} Police;

int selectionMenu(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre);

#endif
