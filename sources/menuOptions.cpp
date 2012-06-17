/*
           ### - PROJET GAME / menuOptions.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 17/06/2012
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

void menuOptions(SDL_Surface *ecran, TTF_Font *police, int largeurFenetre, int hauteurFenetre)
{
    Police okReso, resoX, resoY, resoSeparateur, annuler, avertissementReso, decalGauche, decalDroit, fullScreen; // Variables globales au fichier, préparant les polices

    SDL_Surface *fondEcran(IMG_Load("textures/interface/fondMenu.png"));
    SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));

    int listeResolutionX[6] = {800, 1280, 1366, 1440, 1680, 1920};
    int listeResolutionY[6] = {600, 1024, 768, 900, 1050, 1080};
    int fakeUn(0), fakeDeux(0);
    bool etatFullScreen(false);
    std::string ligne;

    parametresFenetre(&etatFullScreen, &fakeUn, &fakeDeux, false);

    decalGauche.texte = IMG_Load("textures/interface/selectGauche.png"); // Initialisation des textures
    decalDroit.texte = IMG_Load("textures/interface/selectDroit.png");

    SDL_Rect positionCurseur;
    SDL_Rect positionFondEcran;
    positionFondEcran.x = largeurFenetre / 2 - 400;
    positionFondEcran.y = hauteurFenetre / 2 - 300;

    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    int tempsPrecedent(0), tempsActuel = (0), i(0), largeurFenetreMem(largeurFenetre), hauteurFenetreMem(hauteurFenetre);

    while (listeResolutionX[i] != largeurFenetre)
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

    if (etatFullScreen) // Couleur du bouton "Plein écran", qui dépend de l'état de config.cfg
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

    ligne = converteurIntToString(largeurFenetreMem); // Détermination de la résolution utilisée
    resoX.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoX.couleurTexte);

    ligne = converteurIntToString(hauteurFenetreMem);
    resoY.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoY.couleurTexte);

    okReso.texte = TTF_RenderText_Blended(police, "OK", okReso.couleurTexte); // Initialisation des textes
    resoSeparateur.texte = TTF_RenderText_Blended(police, "x", resoSeparateur.couleurTexte);
    annuler.texte = TTF_RenderText_Blended(police, "Annuler", annuler.couleurTexte);
    avertissementReso.texte = TTF_RenderText_Blended(police, "Redémarrez le jeu pour appliquer les changements", avertissementReso.couleurTexte);
    fullScreen.texte = TTF_RenderText_Blended(police, "Plein écran", fullScreen.couleurTexte);

    okReso.positionTexte.x = (largeurFenetre / 4) - (okReso.texte->w / 2); // Positionnement des surfaces écrites
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
            if ((in.sourisX > annuler.positionTexte.x) && (in.sourisX < (annuler.positionTexte.x + annuler.texte->w)) && (in.sourisY > annuler.positionTexte.y) && (in.sourisY < (annuler.positionTexte.y + annuler.texte->h)))
            {
                break;
            }

            else if ((in.sourisX > okReso.positionTexte.x) && (in.sourisX < (okReso.positionTexte.x + okReso.texte->w)) && (in.sourisY > okReso.positionTexte.y) && (in.sourisY < (okReso.positionTexte.y + okReso.texte->h)))
            {
                parametresFenetre(&etatFullScreen, &largeurFenetreMem, &hauteurFenetreMem, true); // Enregistrement des informations sur la résolution et le plein écran
                break;
            }

            else if ((in.sourisX > decalGauche.positionTexte.x) && (in.sourisX < (decalGauche.positionTexte.x + decalGauche.texte->w)) && (in.sourisY > decalGauche.positionTexte.y) && (in.sourisY < (decalGauche.positionTexte.y + decalGauche.texte->h)))
            {
                if (i > 0) // Changement de résolution (moindre)
                {
                    i--;
                    largeurFenetreMem = listeResolutionX[i];
                    hauteurFenetreMem = listeResolutionY[i];
                }
            }

            else if ((in.sourisX > decalDroit.positionTexte.x) && (in.sourisX < (decalDroit.positionTexte.x + decalDroit.texte->w)) && (in.sourisY > decalDroit.positionTexte.y) && (in.sourisY < (decalDroit.positionTexte.y + decalDroit.texte->h)))
            {
                if (i < 5) // Changement de résolution (plus importante)
                {
                    i++;
                    largeurFenetreMem = listeResolutionX[i];
                    hauteurFenetreMem = listeResolutionY[i];
                }
            }

            else if ((in.sourisX > fullScreen.positionTexte.x) && (in.sourisX < (fullScreen.positionTexte.x + fullScreen.texte->w)) && (in.sourisY > fullScreen.positionTexte.y) && (in.sourisY < (fullScreen.positionTexte.y + fullScreen.texte->h)))
            {
                if (etatFullScreen) // Sélection du plein écran
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
        if (tempsActuel - tempsPrecedent > 16) // Mise à jour de l'écran
        {
            SDL_FreeSurface(resoX.texte);
            ligne = converteurIntToString(largeurFenetreMem);
            resoX.texte = TTF_RenderText_Blended(police, ligne.c_str(), resoX.couleurTexte);

            SDL_FreeSurface(resoY.texte);
            ligne = converteurIntToString(hauteurFenetreMem);
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

            SDL_FreeSurface(fullScreen.texte);
            fullScreen.texte = TTF_RenderText_Blended(police, "Plein écran", fullScreen.couleurTexte);

            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 67, 108, 155)); // Effacement de l'écran et affichage des surfaces
            SDL_BlitSurface(fondEcran, NULL, ecran, &positionFondEcran);
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
    SDL_FreeSurface(okReso.texte);
    SDL_FreeSurface(resoSeparateur.texte);
    SDL_FreeSurface(annuler.texte);
    SDL_FreeSurface(avertissementReso.texte);
    SDL_FreeSurface(decalGauche.texte);
    SDL_FreeSurface(decalDroit.texte);

    TTF_CloseFont(police);
}

std::string converteurIntToString(int number) // Convertisseur "int" vers "string"
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
