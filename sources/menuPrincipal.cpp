/*
          ### - PROJET GAME / menuPrincipal.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "menuPrincipal.h"
#include "menuOptions.h"
#include "controlesCamera.h"
#include "majClavier.h"

static Police modeAventure, modeLibre, options, quitter, copyright, terraforma; // Variables globales au fichier, préparant les polices des deux modes de jeu (affichage de la sélection)

static SDL_Surface *fondEcran(IMG_Load("textures/interface/backGround.png")); // Chargement de la texture de fond (texture qui sera répetée)
static SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png")); // Chargement de la texture du curseur personnalisé

int selectionMenu(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre) // Menu principal
{
    ReceptionClavier in; // Création et initialisation de la structure de récupération clavier (suivant une méthode d'un tutoriel du site du zéro -> http://www.siteduzero.com/
    memset(&in, 0, sizeof(in));

    int choixMenu(4), tempsPrecedent(0), tempsActuel(0);

    SDL_Rect positionCurseur;

    SDL_Rect positionFondEcran;
    positionFondEcran.x = 0;
    positionFondEcran.y = 0;

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 30); // Chargement de la police (arial)

    modeAventure.couleurTexte.b = 0; // Initialisation des couleurs (jaune)
    modeAventure.couleurTexte.g = 255;
    modeAventure.couleurTexte.r = 255;
    modeLibre.couleurTexte.b = 0;
    modeLibre.couleurTexte.g = 255;
    modeLibre.couleurTexte.r = 255;
    options.couleurTexte.b = 0;
    options.couleurTexte.g = 255;
    options.couleurTexte.r = 255;
    quitter.couleurTexte.b = 0;
    quitter.couleurTexte.g = 255;
    quitter.couleurTexte.r = 255;
    copyright.couleurTexte.b = 0;
    copyright.couleurTexte.g = 255;
    copyright.couleurTexte.r = 255;
    terraforma.couleurTexte.b = 0;
    terraforma.couleurTexte.g = 255;
    terraforma.couleurTexte.r = 0;

    modeAventure.texte = TTF_RenderText_Blended(police, "Mode Aventure", modeAventure.couleurTexte); // Création des surfaces écrites
    modeLibre.texte = TTF_RenderText_Blended(police, "Mode Libre", modeLibre.couleurTexte);
    options.texte = TTF_RenderText_Blended(police, "Options", options.couleurTexte);
    quitter.texte = TTF_RenderText_Blended(police, "Quitter", quitter.couleurTexte);

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 20); // Chargement de la police (arial, 20 pixels de large)
    copyright.texte = TTF_RenderText_Blended(police, "Copyright 2012, Gianni LADISA--LECLERCQ ©, GPL", copyright.couleurTexte);

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 50); // Chargement de la police (arial, 50 pixels de large)
    terraforma.texte = TTF_RenderText_Blended(police, "TERRAFORMA", terraforma.couleurTexte);

    modeAventure.positionTexte.x = (largeurFenetre / 2) - (modeAventure.texte->w / 2); // Positionnement des surfaces écrites en fonction de la résolution
    modeAventure.positionTexte.y = hauteurFenetre / 4;
    modeLibre.positionTexte.x = (largeurFenetre / 2) - (modeLibre.texte->w / 2);
    modeLibre.positionTexte.y = (hauteurFenetre / 2);
    options.positionTexte.x = 10;
    options.positionTexte.y = (hauteurFenetre - options.texte->h) - 5;
    quitter.positionTexte.x = (largeurFenetre / 2) - (quitter.texte->w / 2);
    quitter.positionTexte.y = (hauteurFenetre / 4) * 3;
    copyright.positionTexte.x = largeurFenetre - copyright.texte->w;
    copyright.positionTexte.y = hauteurFenetre - copyright.texte->h;
    terraforma.positionTexte.x = (largeurFenetre / 2) - (terraforma.texte->w / 2);
    terraforma.positionTexte.y = (hauteurFenetre / 8) - (terraforma.texte->h / 2);

    while (!in.key[SDLK_ESCAPE]) // Boucle de sélection, tant qu'Echap n'est pas pressé
    {
        in.quit = false; // Mise à jour du clavier
        majClavier(&in);

        if (in.quit) // Si la croix est pressée, on quitte le jeu
        {
            choixMenu = 3;
            break;
        }

        if (in.boutonSouris[SDL_BUTTON_LEFT]) // Sélection du menu, mode de jeu, options, etc...
        {
            if ((in.sourisX > options.positionTexte.x) && (in.sourisX < (options.positionTexte.x + options.texte->w)) && (in.sourisY > options.positionTexte.y) && (in.sourisY < (options.positionTexte.y + options.texte->h)))
            {
                menuOptions(ecran, police, largeurFenetre, hauteurFenetre); // Menu des options
                choixMenu = 4;
            }

            else if ((in.sourisX > modeAventure.positionTexte.x) && (in.sourisX < (modeAventure.positionTexte.x + modeAventure.texte->w)) && (in.sourisY > modeAventure.positionTexte.y) && (in.sourisY < (modeAventure.positionTexte.y + modeAventure.texte->h)))
            {
                choixMenu = AVENTURE; // Mode aventure
            }

            else if ((in.sourisX > modeLibre.positionTexte.x) && (in.sourisX < (modeLibre.positionTexte.x + modeLibre.texte->w)) && (in.sourisY > modeLibre.positionTexte.y) && (in.sourisY < (modeLibre.positionTexte.y + modeLibre.texte->h)))
            {
                choixMenu = LIBRE; // Mode libre
            }

            else if ((in.sourisX > quitter.positionTexte.x) && (in.sourisX < (quitter.positionTexte.x + quitter.texte->w)) && (in.sourisY > quitter.positionTexte.y) && (in.sourisY < (quitter.positionTexte.y + quitter.texte->h)))
            {
                choixMenu = 3; // Quitter le jeu
            }
        }

        positionCurseur.x = in.sourisX; // Suivi de la texture du curseur avec la position de la souris
        positionCurseur.y = in.sourisY;

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 16) // Action effectuée toutes les 16 ms
        {
            if (choixMenu == 3 || choixMenu == LIBRE || choixMenu == AVENTURE)
            {
                break;
            }
            else
            {
                SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0)); // Effacement de l'écran et affichage des surfaces

                for (int x(0); x < largeurFenetre; x += fondEcran->w) // Blitage du fond d'écran des menus
                {
                    for (int y(0); y < hauteurFenetre; y += fondEcran->h)
                    {
                        positionFondEcran.x = x;
                        positionFondEcran.y = y;
                        SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
                    }
                }

                SDL_BlitSurface(modeAventure.texte, NULL, ecran, &modeAventure.positionTexte);
                SDL_BlitSurface(modeLibre.texte, NULL, ecran, &modeLibre.positionTexte);
                SDL_BlitSurface(options.texte, NULL, ecran, &options.positionTexte);
                SDL_BlitSurface(quitter.texte, NULL, ecran, &quitter.positionTexte);
                SDL_BlitSurface(copyright.texte, NULL, ecran, &copyright.positionTexte);
                SDL_BlitSurface(terraforma.texte, NULL, ecran, &terraforma.positionTexte);
                SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);
            }

            SDL_Flip(ecran); // Mise à jour de l'écran toutes les 16 ms
            tempsPrecedent = tempsActuel;
        }
    }

    return choixMenu; // Renvoi du choix sous la forme d'un int (voir déclaration des variables plus haut)
}
