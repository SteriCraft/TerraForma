/*
              ### - PROJET GAME / main.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 15/06/2012


    Nombre total de ligne du projet: 3937
*/

#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include "controlesCamera.h" // Fichiers headers propres au programme...
#include "menuPrincipal.h"
#include "menuMaps.h"
#include "lecteurFichier.h"

static TTF_Font *policeTexte(NULL); // Police globale au fichier (fichier de police paramétrable)

int main(int argc, char** argv)
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    int choix(4), largeurFenetre(LARGEUR_FENETRE), hauteurFenetre(PROFONDEUR_FENETRE);
    bool fullscreen(false);

    parametresFenetre(&fullscreen, &largeurFenetre, &hauteurFenetre, false);

    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL...
    SDL_Surface *ecran(NULL);

    if (fullscreen) // Vérification pour le plein écran
    {
        ecran = SDL_SetVideoMode(largeurFenetre,hauteurFenetre,32,SDL_HWSURFACE|SDL_FULLSCREEN);
    }
    else
    {
        ecran = SDL_SetVideoMode(largeurFenetre,hauteurFenetre,32,SDL_HWSURFACE);
    }

    SDL_WM_SetCaption("TerraForma", NULL);
    SDL_EnableKeyRepeat(10,10); // Répétition des touches claviers autorisée
    SDL_ShowCursor(SDL_DISABLE);

    TTF_Init(); // Initialisation du gestionnaire de polices

    Portion_Map world[LARGEUR_MONDE][PROFONDEUR_MONDE];

    initialisationEnvironnement(ecran, world);

    choix = selectionMenu(ecran, policeTexte, largeurFenetre, hauteurFenetre); // Affichage du menu principal

    while (1)
    {
        if (choix == 4)
        {
            choix = selectionMenu(ecran, policeTexte, largeurFenetre, hauteurFenetre); // Affichage du menu principal
        }
        else if (choix == 0 || choix == 1)
        {
            controlesCamera(ecran, choix, largeurFenetre, hauteurFenetre, world, policeTexte);
            //menuMaps(ecran, policeTexte, largeurFenetre, hauteurFenetre, choix);
            choix = 4;
        }
        else
        {
            break;
        }
    }

    suppressionSurface();
    SDL_Quit(); // Arrêt de la SDL
    TTF_Quit();

    return 0; // Arrêt du programme
}
