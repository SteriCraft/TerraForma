/*
              ### - PROJET GAME / main.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012


    Nombre total de ligne du projet: XXXX?
*/

#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include "controlesCamera.h"
#include "menuPrincipal.h"
#include "menuMaps.h"
#include "lecteurFichier.h"

static TTF_Font *policeTexte(NULL); // Police globale au fichier (fichier de police param�trable)

int main(int argc, char** argv)
{
    srand(time(NULL)); // Initialisation du g�n�rateur de nombres al�atoires

    int choix(4), largeurFenetre(LARGEUR_FENETRE), hauteurFenetre(PROFONDEUR_FENETRE); // Initialisation de la fen�tre (r�solution de base)
    bool fullscreen(false);

    parametresFenetre(&fullscreen, &largeurFenetre, &hauteurFenetre, false); // R�cup�ration des param�tres du fichier de configuration, comme le mode plein �cran et la r�solution

    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL...
    SDL_Surface *ecran(NULL);

    if (fullscreen) // V�rification pour le plein �cran
    {
        ecran = SDL_SetVideoMode(largeurFenetre,hauteurFenetre,32,SDL_HWSURFACE|SDL_FULLSCREEN);
    }
    else
    {
        ecran = SDL_SetVideoMode(largeurFenetre,hauteurFenetre,32,SDL_HWSURFACE);
    }

    SDL_WM_SetCaption("TerraForma", NULL); // Titre de la fen�tre
    SDL_EnableKeyRepeat(10,10); // R�p�tition des touches claviers autoris�e
    SDL_ShowCursor(SDL_DISABLE); // Suppression du curseur, qui sera remplac� par une texture propre au jeu

    TTF_Init(); // Initialisation du gestionnaire de polices

    Portion_Map world[LARGEUR_MONDE][PROFONDEUR_MONDE]; // Initialisation de la map

    initialisationEnvironnement(ecran, world); // Cr�ation du monde et de ses composants

    choix = selectionMenu(ecran, policeTexte, largeurFenetre, hauteurFenetre); // Affichage du menu principal

    while (1) // Boucle infinie, arr�t�e par un "break"
    {
        if (choix == 4)
        {
            choix = selectionMenu(ecran, policeTexte, largeurFenetre, hauteurFenetre); // Affichage du menu principal
        }
        else if (choix == 0 || choix == 1)
        {
            controlesCamera(ecran, choix, largeurFenetre, hauteurFenetre, world, policeTexte); // D�marrage du jeu en fonction du mode s�lectionn�
            //menuMaps(ecran, policeTexte, largeurFenetre, hauteurFenetre, choix); Menu de s�lection des maps, � finir
            choix = 4;
        }
        else
        {
            break;
        }
    }

    SDL_Quit(); // Arr�t de la SDL
    TTF_Quit();

    return 0; // Arr�t du programme
}
