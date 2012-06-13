/*
           ### - PROJET GAME / menuOptions.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "menuOptions.h"
#include "menuPrincipal.h"
#include "majClavier.h"
#include "lecteurFichier.h"

static Police okReso, resoX, resoY, resoSeparateur, annuler, avertissementReso, decalGauche, decalDroit, fullScreen; // Variables globales au fichier, pr�parant les polices

static SDL_Surface *fondEcran(IMG_Load("textures/interface/backGround.png")); // Chargement de la texture du fond d'�cran
static SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png")); // Chargement de la texture du curseur

void menuOptions(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre)
{
    int listeResolutionX[6] = {800, 1280, 1366, 1440, 1680, 1920}; // Liste des r�solutions disponibles
    int listeResolutionY[6] = {600, 1024, 768, 900, 1050, 1080};
    int fakeUn(0), fakeDeux(0); // Variables de gestion du code
    bool etatFullScreen(false);
    std::string ligne;

    parametresFenetre(&etatFullScreen, &fakeUn, &fakeDeux, false); // R�cup�ration des param�tres de la fen�tre

    decalGauche.texte = IMG_Load("textures/interface/selectGauche.png"); // Initialisation des textures
    decalDroit.texte = IMG_Load("textures/interface/selectDroit.png");

    SDL_Rect positionCurseur;
    SDL_Rect positionFondEcran;
    positionFondEcran.x = 0;
    positionFondEcran.y = 0;

    ReceptionClavier in; // Mise � jour du clavier
    memset(&in, 0, sizeof(in));

    int tempsPrecedent(0), tempsActuel = (0), i(0), largeurFenetreMem(largeurFenetre), hauteurFenetreMem(hauteurFenetre);

    while (listeResolutionX[i] != largeurFenetre) // D�termination de la r�solution en vigueur
    {
        i++;
    }

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 20); // Chargement de la police (arial)

    okReso.couleurTexte.b = 0; // Initialisation des couleurs (jaune & rouge)
    okReso.couleurTexte.g = 255;
    okReso.couleurTexte.r = 255;
    resoX.couleurTexte.b = 255;
    resoX.couleurTexte.g = 0;
    resoX.couleurTexte.r = 0;
    resoY.couleurTexte.b = 255;
    resoY.couleurTexte.g = 0;
    resoY.couleurTexte.r = 0;
    resoSeparateur.couleurTexte.b = 0;
    resoSeparateur.couleurTexte.g = 255;
    resoSeparateur.couleurTexte.r = 255;
    annuler.couleurTexte.b = 0;
    annuler.couleurTexte.g = 255;
    annuler.couleurTexte.r = 255;
    avertissementReso.couleurTexte.b = 0;
    avertissementReso.couleurTexte.g = 255;
    avertissementReso.couleurTexte.r = 255;
    decalGauche.couleurTexte.b = 0;
    decalGauche.couleurTexte.g = 0;
    decalGauche.couleurTexte.r = 255;
    decalDroit.couleurTexte.b = 0;
    decalDroit.couleurTexte.g = 0;
    decalDroit.couleurTexte.r = 255;

    if (etatFullScreen) // Couleur du bouton "Plein �cran", qui d�pend de l'�tat de config.cfg
    {
        fullScreen.couleurTexte.b = 0;
        fullScreen.couleurTexte.g = 255;
        fullScreen.couleurTexte.r = 0;
    }
    else
    {
        fullScreen.couleurTexte.b = 0;
        fullScreen.couleurTexte.g = 0;
        fullScreen.couleurTexte.r = 255;
    }

    ligne = convertisseurIntToString(largeurFenetreMem); // D�termination de la r�solution utilis�e
    resoX.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoX.couleurTexte);

    ligne = convertisseurIntToString(hauteurFenetreMem);
    resoY.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoY.couleurTexte);

    okReso.texte = TTF_RenderText_Blended(police, "OK", okReso.couleurTexte); // Initialisation des textes
    resoSeparateur.texte = TTF_RenderText_Blended(police, "x", resoSeparateur.couleurTexte);
    annuler.texte = TTF_RenderText_Blended(police, "Annuler", annuler.couleurTexte);
    avertissementReso.texte = TTF_RenderText_Blended(police, "Red�marrez le jeu pour appliquer les changements", avertissementReso.couleurTexte);
    fullScreen.texte = TTF_RenderText_Blended(police, "Plein �cran", fullScreen.couleurTexte);

    okReso.positionTexte.x = (largeurFenetre / 4) - (okReso.texte->w / 2); // Positionnement des surfaces �crites
    okReso.positionTexte.y = hauteurFenetre / 2;
    resoX.positionTexte.x = (largeurFenetre / 4) - (resoX.texte->w / 2);
    resoX.positionTexte.y = (hauteurFenetre / 4) - (resoX.texte->h / 2);
    resoY.positionTexte.x = ((largeurFenetre / 4) * 3) - (resoX.texte->w / 2);
    resoY.positionTexte.y = (hauteurFenetre / 4) - (resoX.texte->h / 2);
    resoSeparateur.positionTexte.x = (largeurFenetre / 2) - (resoSeparateur.texte->w / 2);
    resoSeparateur.positionTexte.y = (hauteurFenetre / 4) - (resoSeparateur.texte->h / 2);
    annuler.positionTexte.x = ((largeurFenetre / 4) * 3) - (annuler.texte->w / 2);
    annuler.positionTexte.y = hauteurFenetre / 2;
    avertissementReso.positionTexte.x = (largeurFenetre / 2) - (avertissementReso.texte->w / 2);
    avertissementReso.positionTexte.y = (hauteurFenetre / 4) * 3;
    decalGauche.positionTexte.x = ((largeurFenetre / 4) - (resoX.texte->w / 2)) - 40;
    decalGauche.positionTexte.y = resoX.positionTexte.y;
    decalDroit.positionTexte.x = (((largeurFenetre / 4) * 3) - (resoX.texte->w / 2)) + 40;
    decalDroit.positionTexte.y = resoX.positionTexte.y;
    fullScreen.positionTexte.x = (largeurFenetre / 2) - (fullScreen.texte->w / 2);
    fullScreen.positionTexte.y = (hauteurFenetre / 2) - (fullScreen.texte->h / 2);

    while (!in.key[SDLK_ESCAPE]) // Boucle de s�lection
    {
        in.quit = false;
        majClavier(&in);

        if (in.quit)
        {
            break;
        }

        if (in.boutonSouris[SDL_BUTTON_LEFT])
        {
            if ((in.sourisX > annuler.positionTexte.x) && (in.sourisX < (annuler.positionTexte.x + annuler.texte->w)) && (in.sourisY > annuler.positionTexte.y) && (in.sourisY < (annuler.positionTexte.y + annuler.texte->h)))
            {
                break; // Quitter le menu
            }

            else if ((in.sourisX > okReso.positionTexte.x) && (in.sourisX < (okReso.positionTexte.x + okReso.texte->w)) && (in.sourisY > okReso.positionTexte.y) && (in.sourisY < (okReso.positionTexte.y + okReso.texte->h)))
            {
                parametresFenetre(&etatFullScreen, &largeurFenetreMem, &hauteurFenetreMem, true); // Enregistrement des informations sur la r�solution et le plein �cran
                break;
            }

            else if ((in.sourisX > decalGauche.positionTexte.x) && (in.sourisX < (decalGauche.positionTexte.x + decalGauche.texte->w)) && (in.sourisY > decalGauche.positionTexte.y) && (in.sourisY < (decalGauche.positionTexte.y + decalGauche.texte->h)))
            {
                if (i > 0) // Changement de r�solution (moindre)
                {
                    i--;
                    largeurFenetreMem = listeResolutionX[i];
                    hauteurFenetreMem = listeResolutionY[i];
                }
            }

            else if ((in.sourisX > decalDroit.positionTexte.x) && (in.sourisX < (decalDroit.positionTexte.x + decalDroit.texte->w)) && (in.sourisY > decalDroit.positionTexte.y) && (in.sourisY < (decalDroit.positionTexte.y + decalDroit.texte->h)))
            {
                if (i < 5) // Changement de r�solution (plus importante)
                {
                    i++;
                    largeurFenetreMem = listeResolutionX[i];
                    hauteurFenetreMem = listeResolutionY[i];
                }
            }

            else if ((in.sourisX > fullScreen.positionTexte.x) && (in.sourisX < (fullScreen.positionTexte.x + fullScreen.texte->w)) && (in.sourisY > fullScreen.positionTexte.y) && (in.sourisY < (fullScreen.positionTexte.y + fullScreen.texte->h)))
            {
                if (etatFullScreen) // S�lection du plein �cran
                {
                    etatFullScreen = false;
                }
                else
                {
                    etatFullScreen = true;
                }
            }
        }

        positionCurseur.x = in.sourisX;
        positionCurseur.y = in.sourisY;

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 16) // Mise � jour de l'�cran
        {
            ligne = convertisseurIntToString(largeurFenetreMem);
            resoX.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoX.couleurTexte);

            ligne = convertisseurIntToString(largeurFenetreMem);
            resoX.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoX.couleurTexte);

            ligne = convertisseurIntToString(hauteurFenetreMem);
            resoY.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoY.couleurTexte);

            resoX.positionTexte.x = (largeurFenetre / 4) - (resoX.texte->w / 2);
            resoX.positionTexte.y = (hauteurFenetre / 4) - (resoX.texte->h / 2);
            resoY.positionTexte.x = ((largeurFenetre / 4) * 3) - (resoX.texte->w / 2);
            resoY.positionTexte.y = (hauteurFenetre / 4) - (resoX.texte->h / 2);
            decalGauche.positionTexte.x = ((largeurFenetre / 4) - (resoX.texte->w / 2)) - 60;
            decalGauche.positionTexte.y = resoX.positionTexte.y;
            decalDroit.positionTexte.x = (((largeurFenetre / 4) * 3) - (resoX.texte->w / 2)) + 60;
            decalDroit.positionTexte.y = resoX.positionTexte.y;

            if (etatFullScreen)
            {
                fullScreen.couleurTexte.b = 0;
                fullScreen.couleurTexte.g = 255;
                fullScreen.couleurTexte.r = 0;
            }
            else
            {
                fullScreen.couleurTexte.b = 0;
                fullScreen.couleurTexte.g = 0;
                fullScreen.couleurTexte.r = 255;
            }

            fullScreen.texte = TTF_RenderText_Blended(police, "Plein �cran", fullScreen.couleurTexte);

            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0)); // Effacement de l'�cran et affichage des surfaces

            for (int x(0); x < largeurFenetre; x += fondEcran->w)
            {
                for (int y(0); y < hauteurFenetre; y += fondEcran->h)
                {
                    positionFondEcran.x = x;
                    positionFondEcran.y = y;
                    SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
                }
            }

            SDL_BlitSurface(okReso.texte, NULL, ecran, &okReso.positionTexte);
            SDL_BlitSurface(resoX.texte, NULL, ecran, &resoX.positionTexte);
            SDL_BlitSurface(resoY.texte, NULL, ecran, &resoY.positionTexte);
            SDL_BlitSurface(resoSeparateur.texte, NULL, ecran, &resoSeparateur.positionTexte);
            SDL_BlitSurface(annuler.texte, NULL, ecran, &annuler.positionTexte);
            SDL_BlitSurface(avertissementReso.texte, NULL, ecran, &avertissementReso.positionTexte);
            SDL_BlitSurface(decalGauche.texte, NULL, ecran, &decalGauche.positionTexte);
            SDL_BlitSurface(decalDroit.texte, NULL, ecran, &decalDroit.positionTexte);
            SDL_BlitSurface(fullScreen.texte, NULL, ecran, &fullScreen.positionTexte);
            SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);

            SDL_Flip(ecran); // Mise � jour de l'�cran toutes les 16 ms

            tempsPrecedent = tempsActuel;
        }
    }
}

std::string convertisseurIntToString(int number) // Convertisseur "int" vers "string"
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}