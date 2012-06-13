#include <SDL_image.h>
#include "menuPrincipal.h"
#include "menuMaps.h"
#include "majClavier.h"
#include "gestionMap.h"

static SDL_Surface *fondEcran(IMG_Load("textures/interface/backGround.png"));
static SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));

static Police nouvellePartie, chargerPartie, supprimerPartie, annuler;

void menuMaps(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre, int modeJeu)
{
    int tempsActuel(0), tempsPrecedent(0), nombreMaps(0);

    SDL_Rect positionFondEcran;
    SDL_Rect positionCurseur;

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 30);

    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    nouvellePartie.couleurTexte.b = 255;
    nouvellePartie.couleurTexte.g = 0;
    nouvellePartie.couleurTexte.r = 0;
    chargerPartie.couleurTexte.b = 255;
    chargerPartie.couleurTexte.g = 0;
    chargerPartie.couleurTexte.r = 0;
    supprimerPartie.couleurTexte.b = 255;
    supprimerPartie.couleurTexte.g = 0;
    supprimerPartie.couleurTexte.r = 0;
    annuler.couleurTexte.b = 255;
    annuler.couleurTexte.g = 0;
    annuler.couleurTexte.r = 0;

    nouvellePartie.texte = TTF_RenderText_Blended(police, "Nouvelle partie", nouvellePartie.couleurTexte);
    chargerPartie.texte = TTF_RenderText_Blended(police, "Charger cette partie", chargerPartie.couleurTexte);
    supprimerPartie.texte = TTF_RenderText_Blended(police, "Supprimer cette partie", supprimerPartie.couleurTexte);
    annuler.texte = TTF_RenderText_Blended(police, "Annuler", annuler.couleurTexte);

    nouvellePartie.positionTexte.x = (largeurFenetre / 4) - (nouvellePartie.texte->w / 2);
    nouvellePartie.positionTexte.y = ((hauteurFenetre / 5) * 4) - (nouvellePartie.texte->w / 2);
    chargerPartie.positionTexte.x = ((largeurFenetre / 4) * 3) - (nouvellePartie.texte->w / 2);
    chargerPartie.positionTexte.y = ((hauteurFenetre / 5) * 4) - (nouvellePartie.texte->w / 2);
    supprimerPartie.positionTexte.x = (largeurFenetre / 4) - (nouvellePartie.texte->w / 2);
    supprimerPartie.positionTexte.y = ((hauteurFenetre / 5) * 4) - (nouvellePartie.texte->w / 2) + 50;
    annuler.positionTexte.x = ((largeurFenetre / 4) * 3) - (nouvellePartie.texte->w / 2);
    annuler.positionTexte.y = ((hauteurFenetre / 5) * 4) - (nouvellePartie.texte->w / 2) + 50;

    while (!in.key[SDLK_ESCAPE]) // Boucle de sélection
    {
        in.quit = false;
        majClavier(&in);

        if (in.quit)
        {
            break;
        }

        if (in.boutonSouris[SDL_BUTTON_LEFT])
        {
            if (in.sourisX > nouvellePartie.positionTexte.x && in.sourisX < nouvellePartie.positionTexte.x + nouvellePartie.texte->w)
            {
                if (in.sourisY > nouvellePartie.positionTexte.y && in.sourisY < nouvellePartie.positionTexte.y + nouvellePartie.texte->h)
                {
                    if (nombreMaps < 4)
                    {
                        creerMap(ecran, nombreMaps + 1, modeJeu, largeurFenetre, hauteurFenetre, police);
                        break;
                    }
                }
            }
            else if (in.sourisX > chargerPartie.positionTexte.x && in.sourisX < chargerPartie.positionTexte.x + chargerPartie.texte->w)
            {
                if (in.sourisY > chargerPartie.positionTexte.y && in.sourisY < chargerPartie.positionTexte.y + chargerPartie.texte->h)
                {
                    chargerMap(ecran, nombreMaps + 1, modeJeu, largeurFenetre, hauteurFenetre, police);
                    break;
                }
            }
            else if (in.sourisX > annuler.positionTexte.x && in.sourisX < annuler.positionTexte.x + annuler.texte->w)
            {
                if (in.sourisY > annuler.positionTexte.y && in.sourisY < annuler.positionTexte.y + annuler.texte->h)
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
            for (int x(0); x < largeurFenetre; x += fondEcran->w)
            {
                for (int y(0); y < hauteurFenetre; y += fondEcran->h)
                {
                    positionFondEcran.x = x;
                    positionFondEcran.y = y;
                    SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
                }
            }

            SDL_BlitSurface(nouvellePartie.texte, NULL, ecran, &nouvellePartie.positionTexte);
            SDL_BlitSurface(chargerPartie.texte, NULL, ecran, &chargerPartie.positionTexte);
            SDL_BlitSurface(supprimerPartie.texte, NULL, ecran, &supprimerPartie.positionTexte);
            SDL_BlitSurface(annuler.texte, NULL, ecran, &annuler.positionTexte);
            SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);

            SDL_Flip(ecran);

            tempsPrecedent = tempsActuel;
        }
    }
}
