#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "menuMort.h"
#include "menuPrincipal.h"
#include "majClavier.h"

void menuMort(SDL_Surface *ecran, int largeurFenetre, int hauteurFenetre, TTF_Font *police)
{
    Police annonceMort, menu;

    SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));

    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    int tempsActuel(0), tempsPrecedent(0);

    SDL_Rect positionCurseur;

    annonceMort.couleurTexte.b = 255;
    annonceMort.couleurTexte.g = 0;
    annonceMort.couleurTexte.r = 0;
    menu.couleurTexte.b = 0;
    menu.couleurTexte.g = 255;
    menu.couleurTexte.r = 0;

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 50); // Chargement de la police (arial)
    annonceMort.texte = TTF_RenderText_Blended(police, "Vous êtes mort", annonceMort.couleurTexte);

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 30); // Chargement de la police (arial)
    menu.texte = TTF_RenderText_Blended(police, "Menu principal", menu.couleurTexte);

    annonceMort.positionTexte.x = (largeurFenetre / 2) - (annonceMort.texte->w / 2);
    annonceMort.positionTexte.y = (hauteurFenetre / 4) - (annonceMort.texte->h / 2);
    menu.positionTexte.x = (largeurFenetre / 2) - (menu.texte->w / 2);
    menu.positionTexte.y = ((hauteurFenetre / 4) * 3) - (menu.texte->h / 2);

    while (!in.key[SDLK_ESCAPE]) // Boucle de sélection
    {
        in.quit = false;
        majClavier(&in, false);

        if (in.quit)
        {
            break;
        }

        if (in.boutonSouris[SDL_BUTTON_LEFT])
        {
            if (in.sourisX > menu.positionTexte.x && in.sourisX < menu.positionTexte.x + menu.texte->w)
            {
                if (in.sourisY > menu.positionTexte.y && in.sourisY < menu.positionTexte.y + menu.texte->h)
                {
                    break;
                }
            }
        }

        positionCurseur.x = in.sourisX;
        positionCurseur.y = in.sourisY;

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 16) // Mise à jour de l'écran
        {
            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 128, 0));
            SDL_BlitSurface(annonceMort.texte, NULL, ecran, &annonceMort.positionTexte);
            SDL_BlitSurface(menu.texte, NULL, ecran, &menu.positionTexte);
            SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);

            SDL_Flip(ecran); // Mise à jour de l'écran toutes les 16 ms

            tempsPrecedent = tempsActuel;
        }
        else
        {
            SDL_Delay(16 - (tempsActuel - tempsPrecedent));
        }
    }

    SDL_FreeSurface(annonceMort.texte);
    SDL_FreeSurface(menu.texte);
    SDL_FreeSurface(curseur);

    TTF_CloseFont(police);
}
