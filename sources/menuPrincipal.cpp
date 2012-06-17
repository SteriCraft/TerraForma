/*
          ### - PROJET GAME / menuPrincipal.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 17/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>

#include "menuPrincipal.h"
#include "menuOptions.h"
#include "controlesCamera.h"
#include "majClavier.h"

int selectionMenu(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre)
{
    FMOD_SYSTEM *system;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);

    FMOD_SOUND *clic(NULL), *musique(NULL);
    FMOD_System_CreateSound(system, "sons/interface/clicMenu.wma", FMOD_CREATESAMPLE, 0, &clic);

    FMOD_System_CreateSound(system, "sons/interface/musiqueMenu.wma", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &musique);

    FMOD_CHANNEL *channel;
    FMOD_System_GetChannel(system, 2, &channel);

    FMOD_Sound_SetLoopCount(musique, -1);

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL);

    SDL_Surface *fondEcran(IMG_Load("textures/interface/fondMenu.png"));
    SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));
    SDL_Surface *modeAventure(IMG_Load("textures/interface/btn_aventure.png"));
    SDL_Surface *modeLibre(IMG_Load("textures/interface/btn_libre.png"));
    SDL_Surface *options(IMG_Load("textures/interface/btn_options.png"));
    SDL_Surface *quitter(IMG_Load("textures/interface/btn_quitter.png"));
    SDL_Surface *credits(IMG_Load("textures/interface/btn_credits.png"));

    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    int choixMenu(4), tempsPrecedent(0), tempsActuel(0);

    SDL_Rect positionCurseur;
    SDL_Rect positionModeLibre;
    SDL_Rect positionModeAventure;
    SDL_Rect positionOptions;
    SDL_Rect positionCredits;
    SDL_Rect positionQuitter;
    SDL_Rect positionFondEcran;
    positionFondEcran.x = largeurFenetre / 2 - 400;
    positionFondEcran.y = hauteurFenetre / 2 - 300;

    positionModeAventure.x = (largeurFenetre / 2) - 125; // Positionnement des surfaces écrites
    positionModeAventure.y = (hauteurFenetre / 2) - 75;
    positionModeLibre.x = (largeurFenetre / 2) - 125;
    positionModeLibre.y = (hauteurFenetre / 2) - 5;
    positionOptions.x = (largeurFenetre / 2) - 125;
    positionOptions.y = (hauteurFenetre / 2) + 65;
    positionQuitter.x = (largeurFenetre / 2) - 125;
    positionQuitter.y = (hauteurFenetre / 2) + 205;
    positionCredits.x = (largeurFenetre / 2) - 125;
    positionCredits.y = (hauteurFenetre / 2) + 135;

    while (!in.key[SDLK_ESCAPE]) // Boucle de sélection
    {
        in.quit = false;
        majClavier(&in, false);

        if (in.quit)
        {
            choixMenu = 3;
            break;
        }

        if (in.boutonSouris[SDL_BUTTON_LEFT]) // Sélection du menu, mode de jeu, options, etc...
        {
            if ((in.sourisX > positionOptions.x) && (in.sourisX < (positionOptions.x + 250)) && (in.sourisY > positionOptions.y) && (in.sourisY < (positionOptions.y + 65)))
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, clic, 0, NULL);
                SDL_Delay(800);
                menuOptions(ecran, police, largeurFenetre, hauteurFenetre);
                choixMenu = 4;
            }

            else if ((in.sourisX > positionModeAventure.x) && (in.sourisX < (positionModeAventure.x + 250)) && (in.sourisY > positionModeAventure.y) && (in.sourisY < (positionModeAventure.y + 65)))
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, clic, 0, NULL);
                SDL_Delay(800);
                choixMenu = AVENTURE;
            }

            else if ((in.sourisX > positionModeLibre.x) && (in.sourisX < (positionModeLibre.x + 250)) && (in.sourisY > positionModeLibre.y) && (in.sourisY < (positionModeLibre.y + 65)))
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, clic, 0, NULL);
                SDL_Delay(800);
                choixMenu = LIBRE;
            }

            else if ((in.sourisX > positionQuitter.x) && (in.sourisX < (positionQuitter.x + 250)) && (in.sourisY > positionQuitter.y) && (in.sourisY < (positionQuitter.y + 65)))
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, clic, 0, NULL);
                SDL_Delay(800);
                choixMenu = 3;
            }
        }

        positionCurseur.x = in.sourisX;
        positionCurseur.y = in.sourisY;

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 16)
        {
            if (choixMenu == 3 || choixMenu == LIBRE || choixMenu == AVENTURE)
            {
                break;
            }
            else
            {
                SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 67, 108, 155)); // Effacement de l'écran et affichage des surfaces
                SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
                SDL_BlitSurface(modeAventure, NULL, ecran, &positionModeAventure);
                SDL_BlitSurface(modeLibre, NULL, ecran, &positionModeLibre);
                SDL_BlitSurface(options, NULL, ecran, &positionOptions);
                SDL_BlitSurface(quitter, NULL, ecran, &positionQuitter);
                SDL_BlitSurface(credits, NULL, ecran, &positionCredits);
                SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);
            }

            SDL_Flip(ecran); // Mise à jour de l'écran toutes les 16 ms
            tempsPrecedent = tempsActuel;
        }
        else
        {
            SDL_Delay(16 - (tempsActuel - tempsPrecedent));
        }
    }

    SDL_FreeSurface(curseur);
    SDL_FreeSurface(fondEcran);
    SDL_FreeSurface(modeAventure);
    SDL_FreeSurface(modeLibre);
    SDL_FreeSurface(options);
    SDL_FreeSurface(quitter);
    SDL_FreeSurface(credits);

    FMOD_Sound_Release(clic);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return choixMenu; // Renvoi du choix sous la forme d'un int (voir déclaration des variables plus haut)
}
