/*
           ### - PROJET GAME / interface.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "interface.h"
#include "menuPrincipal.h"
#include "environnement.h"
#include "menuOptions.h"

static Police quantiteItemsPolice;

InterfaceJeu::InterfaceJeu(int largeurFenetre, int modeJeu) : selectionBloc(HERBE) // Constructeur de l'interface en jeu
{
    int x(0);

    barreInventaire = IMG_Load("textures/interface/barreInventaire.png"); // Chargement des surfaces pour l'interface
    indicateursPerso = IMG_Load("textures/interface/indicateursPerso.png");
    tetePerso = IMG_Load("textures/skins/kevinMiniature.png");
    barreVieUn = IMG_Load("textures/interface/barreVieUn.png");
    barreVieDeux = IMG_Load("textures/interface/barreVieDeux.png");
    barreVieTrois = IMG_Load("textures/interface/barreVieTrois.png");
    barreVieQuatre = IMG_Load("textures/interface/barreVieQuatre.png");
    barreVieCinq = IMG_Load("textures/interface/barreVieCinq.png");
    barreMana = IMG_Load("textures/interface/barreMana.png");
    iconeInventaire = IMG_Load("textures/interface/iconeInventaire.png");
    selection = IMG_Load("textures/interface/selectionBloc.png");
    croix = IMG_Load("textures/interface/croix.png");
    imageInventaire = IMG_Load("textures/interface/inventaire.png");

    positionBarreInventaire.x = (largeurFenetre - barreInventaire->w) - 160; // Positionnement de ces dernières
    positionBarreInventaire.y = 0;

    positionIndicateursPerso.x = largeurFenetre - indicateursPerso->w;
    positionIndicateursPerso.y = 0;

    positionTetePerso.x = (largeurFenetre - tetePerso->w) - 10;
    positionTetePerso.y = 10;

    positionBarreVie.x = (largeurFenetre - barreVieUn->w) - 144;
    positionBarreVie.y = 0;

    positionBarreMana.x = (largeurFenetre - barreMana->w);
    positionBarreMana.y = 136;

    positionIconeInventaire.x = largeurFenetre - 245;
    positionIconeInventaire.y = 29;

    positionSelection.x = largeurFenetre - 566;
    positionSelection.y = 28;

    positionNombreItems.x = largeurFenetre - 566;
    positionNombreItems.y = 28;

    x = largeurFenetre - 565;

    for (int i(0); i < 4; i++) // Positionnements des items de la main (en haut à droite de l'écran)
    {
        positionItems[i].x = x;
        positionItems[i].y = 29;

        x += 80;
    }

    chargerTexturesItems(); // Chargement des textures

    if (modeJeu == 1) // Initialisation de l'inventaire en mode Libre
    {
        inventaire[0][0] = HERBE;
        inventaire[0][1] = TERRE;
        inventaire[0][2] = PIERRE;
        inventaire[0][3] = CHARBON;
        inventaire[0][4] = FER;
        inventaire[0][5] = BOIS;
        inventaire[0][6] = BOIS_NATUREL;
        inventaire[0][7] = FEUILLE;
        inventaire[0][8] = AIR;
        inventaire[0][9] = AIR;
        inventaire[0][10] = AIR;

        for (int a(1); a < 8; a++) //
        {
            for (int b(0); b < 10; b++)
            {
                inventaire[a][b] = AIR;
            }
        }
    }
    else
    {
        for (int a(0); a < 8; a++) // Ou vidage de celui-ci en mode Aventure
        {
            for (int b(0); b < 10; b++)
            {
                inventaire[a][b] = AIR;
            }
        }
    }

    for (int a(0); a < 8; a++) // Initialisation des variables de contrôle de l'inventaire (quantité max des stacks, etc...)
    {
        for (int b(0); b < 10; b++)
        {
            if (inventaire[a][b] == AIR)
            {
                nombreItemsDeux[a][b] = 0;
            }
            else
            {
                if (inventaire[a][b] == HERBE)
                {
                    quantiteMaxItems[a][b] = MAX_HERBE;
                }
                else if (inventaire[a][b] == TERRE)
                {
                    quantiteMaxItems[a][b] = MAX_TERRE;
                }
                else if (inventaire[a][b] == PIERRE)
                {
                    quantiteMaxItems[a][b] = MAX_PIERRE;
                }
                else if (inventaire[a][b] == CHARBON)
                {
                    quantiteMaxItems[a][b] = MAX_CHARBON;
                }
                else if (inventaire[a][b] == FER)
                {
                    quantiteMaxItems[a][b] = MAX_FER;
                }
                else if (inventaire[a][b] == BOIS)
                {
                    quantiteMaxItems[a][b] = MAX_BOIS;
                }
                else if (inventaire[a][b] == BOIS_NATUREL)
                {
                    quantiteMaxItems[a][b] = MAX_BOIS_NATUREL;
                }
                else if (inventaire[a][b] == FEUILLE)
                {
                    quantiteMaxItems[a][b] = MAX_FEUILLE;
                }

                nombreItemsDeux[a][b] = 1;
            }
        }
    }

    if (modeJeu == 1)
    {
        selectionInventaire[0] = inventaire[0][0]; // Initialisatio de la main en mode Libre
        selectionInventaire[1] = inventaire[0][1];
        selectionInventaire[2] = inventaire[0][2];
        selectionInventaire[3] = inventaire[0][5];
    }
}

InterfaceJeu::~InterfaceJeu() // Destructeur de la classe
{
}

void InterfaceJeu::afficherInterface(SDL_Surface *ecran, int viePerso) // Affichage de l'interface
{
    SDL_BlitSurface(barreInventaire, NULL, ecran, &positionBarreInventaire);
    SDL_BlitSurface(indicateursPerso, NULL, ecran, &positionIndicateursPerso);
    SDL_BlitSurface(tetePerso, NULL, ecran, &positionTetePerso);

    if (viePerso >= 80) // Affichage du niveau de vie
    {
        SDL_BlitSurface(barreVieUn, NULL, ecran, &positionBarreVie);
    }
    else if (viePerso >= 60 && viePerso < 80)
    {
        SDL_BlitSurface(barreVieDeux, NULL, ecran, &positionBarreVie);
    }
    else if (viePerso >= 40 && viePerso < 60)
    {
        SDL_BlitSurface(barreVieTrois, NULL, ecran, &positionBarreVie);
    }
    else if (viePerso >= 20 && viePerso < 40)
    {
        SDL_BlitSurface(barreVieQuatre, NULL, ecran, &positionBarreVie);
    }
    else if (viePerso > 0 && viePerso < 20)
    {
        SDL_BlitSurface(barreVieCinq, NULL, ecran, &positionBarreVie);
    }

    SDL_BlitSurface(barreMana, NULL, ecran, &positionBarreMana);
    SDL_BlitSurface(iconeInventaire, NULL, ecran, &positionIconeInventaire);

    for (int i(0); i < 4; i++) // Affichage de la main
    {
        if (selectionInventaire[i] == HERBE)
        {
            SDL_BlitSurface(items[HERBE - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == TERRE)
        {
            SDL_BlitSurface(items[TERRE - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == PIERRE)
        {
            SDL_BlitSurface(items[PIERRE - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == CHARBON)
        {
            SDL_BlitSurface(items[CHARBON - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == FER)
        {
            SDL_BlitSurface(items[FER - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == BOIS)
        {
            SDL_BlitSurface(items[BOIS - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == BOIS_NATUREL)
        {
            SDL_BlitSurface(items[BOIS_NATUREL - 1], NULL, ecran, &positionItems[i]);
        }
        else if (selectionInventaire[i] == FEUILLE)
        {
            SDL_BlitSurface(items[FEUILLE - 1], NULL, ecran, &positionItems[i]);
        }
    }

    SDL_BlitSurface(selection, NULL, ecran, &positionSelection);
}

void InterfaceJeu::chargerTexturesItems() // Chargement des textures (fonction séparée)
{
    items[TERRE - 1] = IMG_Load("textures/items/terre.png");
    items[PIERRE - 1] = IMG_Load("textures/items/pierre.png");
    items[CHARBON - 1] = IMG_Load("textures/items/charbon.png");
    items[FER - 1] = IMG_Load("textures/items/fer.png");
    items[HERBE - 1] = IMG_Load("textures/items/herbe.png");
    items[BOIS - 1] = IMG_Load("textures/items/bois.png");
    items[BOIS_NATUREL - 1] = IMG_Load("textures/items/bois_naturel.png");
    items[FEUILLE - 1] = IMG_Load("textures/items/feuille.png");
}

int InterfaceJeu::blocSelectionner() // Sélection de bloc
{
    if (selectionBloc == 1) // Récupération du type de bloc sélectionner par le curseur de la main (carré marron)
    {
        return selectionInventaire[0];
    }
    else if (selectionBloc == 2)
    {
        return selectionInventaire[1];
    }
    else if (selectionBloc == 3)
    {
        return selectionInventaire[2];
    }
    else if (selectionBloc == 4)
    {
        return selectionInventaire[3];
    }
    else
    {
        return AIR; // En cas d'échec de l'identification, on renvoi de l'air
    }
}

void InterfaceJeu::changerPositionSelection(bool direction, int largeurFenetre) // Modification de la sélection de bloc
{
    if (direction && selectionBloc < 4) // Modification du bloc sélectionner par le curseur de la main
    {
        selectionBloc++;
    }
    else if (!direction && selectionBloc > 1)
    {
        selectionBloc--;
    }

    if (selectionBloc == 1) // Et positionnement de ce dernier
    {
        positionSelection.x = largeurFenetre - 566;
    }
    else if (selectionBloc == 2)
    {
        positionSelection.x = largeurFenetre - 486;
    }
    else if (selectionBloc == 3)
    {
        positionSelection.x = largeurFenetre - 406;
    }
    else if (selectionBloc == 4)
    {
        positionSelection.x = largeurFenetre - 326;
    }
}

void InterfaceJeu::afficherInventaire(SDL_Surface *ecran, int largeurFenetre, int hauteurFenetre) // Affichage de l'inventaire
{
    positionInventaire.x = (largeurFenetre / 2) - (imageInventaire->w / 2); // Positionnement de l'inventaire
    positionInventaire.y = (hauteurFenetre / 2) - (imageInventaire->h / 2);
    positionCroix.x = positionInventaire.x + 760;
    positionCroix.y = positionInventaire.y + 25;

    SDL_BlitSurface(imageInventaire, NULL, ecran, &positionInventaire); // Blitage de l'inventaire
    SDL_BlitSurface(croix, NULL, ecran, &positionCroix);

    for (int x(0); x < 10; x++) // Blitage des blocs qu'il contient, en fonction de leur type (tableau de 8 lignes de 10 colonnes)
    {
        for (int y(0); y < 8; y++)
        {
            positionBloc.x = (x * TAILLE_BLOCK * 4.3) + (positionInventaire.x + 73);
            positionBloc.y = (y * TAILLE_BLOCK * 3.4) + (positionInventaire.y + 130);

            if (inventaire[y][x] == AIR)
            {
                blocInventaire = IMG_Load("textures/items/air.png");
            }
            else if (inventaire[y][x] == HERBE)
            {
                blocInventaire = IMG_Load("textures/items/herbe.png");
            }
            else if (inventaire[y][x] == TERRE)
            {
                blocInventaire = IMG_Load("textures/items/terre.png");
            }
            else if (inventaire[y][x] == PIERRE)
            {
                blocInventaire = IMG_Load("textures/items/pierre.png");
            }
            else if (inventaire[y][x] == CHARBON)
            {
                blocInventaire = IMG_Load("textures/items/charbon.png");
            }
            else if (inventaire[y][x] == FER)
            {
                blocInventaire = IMG_Load("textures/items/fer.png");
            }
            else if (inventaire[y][x] == BOIS)
            {
                blocInventaire = IMG_Load("textures/items/bois.png");
            }
            else if (inventaire[y][x] == BOIS_NATUREL)
            {
                blocInventaire = IMG_Load("textures/items/bois_naturel.png");
            }
            else if (inventaire[y][x] == FEUILLE)
            {
                blocInventaire = IMG_Load("textures/items/feuille.png");
            }

            SDL_BlitSurface(blocInventaire, NULL, ecran, &positionBloc);
        }
    }
}

void InterfaceJeu::testClicInventaire(int posX, int posY) // Vérification de l'impact des clics dans l'inventaire sur les blocs qu'il contient
{
    int posBlocX(0), posBlocY(0);

    for (int x(0); x < 8; x++)
    {
        for (int y(0); y < 10; y++)
        {
            posBlocX = (y * TAILLE_BLOCK * 4.3) + (positionInventaire.x + 73);
            posBlocY = (x * TAILLE_BLOCK * 3.4) + (positionInventaire.y + 130);

            if (posX > posBlocX && posX < posBlocX + 32)
            {
                if (posY > posBlocY && posY < posBlocY + 32)
                {
                    if (selectionBloc == 1)
                    {
                        selectionInventaire[0] = inventaire[x][y];
                    }
                    else if (selectionBloc == 2)
                    {
                        selectionInventaire[1] = inventaire[x][y];
                    }
                    else if (selectionBloc == 3)
                    {
                        selectionInventaire[2] = inventaire[x][y];
                    }
                    else if (selectionBloc == 4)
                    {
                        selectionInventaire[3] = inventaire[x][y];
                    }
                }
            }
        }
    }
}

void InterfaceJeu::afficherCompteurBlocInventaire(int largeurFenetre, int hauteurFenetre, TTF_Font *police, SDL_Surface *ecran) // Affichage du nombre de blocs dans les stacks (en mode Aventure uniquement)
{
    std::string quantiteItems("");

    police = TTF_OpenFont("textures/interface/policeMenu.ttf", 20);

    quantiteItemsPolice.couleurTexte.r = 255;
    quantiteItemsPolice.couleurTexte.b = 255;
    quantiteItemsPolice.couleurTexte.g = 255;


    for (int x(0); x < 8; x++)
    {
        for (int y(0); y < 10; y++)
        {
            if (nombreItemsDeux[x][y] > 1)
            {
                if (nombreItemsDeux[x][y] > quantiteMaxItems[x][y])
                {
                    nombreItemsDeux[x][y] = quantiteMaxItems[x][y];
                }

                quantiteItems = convertisseurIntToString(nombreItemsDeux[x][y]);

                quantiteItemsPolice.texte = TTF_RenderText_Blended(police, quantiteItems.c_str(), quantiteItemsPolice.couleurTexte);

                quantiteItemsPolice.positionTexte.x = (y * TAILLE_BLOCK * 4.3) + (positionInventaire.x + 73) + 6;
                quantiteItemsPolice.positionTexte.y = (x * TAILLE_BLOCK * 3.4) + (positionInventaire.y + 130) - 1;

                SDL_BlitSurface(quantiteItemsPolice.texte, NULL, ecran, &quantiteItemsPolice.positionTexte);
            }
        }
    }
}

void InterfaceJeu::ajouterEnleverBlocInventaire(int typeBloc, bool modifier, bool *ok) // Modification du nombre de blocs dans les stacks de l'inventaire (mode Aventure uniquement)
{
    int posX(0), posY(0);
    bool trouver(false);

    if (typeBloc != AIR && modifier)
    {
        for (int x(0); x < 8; x++)
        {
            for (int y(0); y < 10; y++)
            {
                if (inventaire[x][y] == typeBloc && nombreItemsDeux[x][y] < quantiteMaxItems[x][y])
                {
                    posX = x;
                    posY = y;
                    trouver = true;
                    break;
                }
            }

            if (trouver)
            {
                break;
            }
        }

        if (trouver)
        {
            nombreItemsDeux[posX][posY]++;
        }
        else
        {
            for (int x(0); x < 8; x++)
            {
                for (int y(0); y < 10; y++)
                {
                    if (inventaire[x][y] == AIR)
                    {
                        inventaire[x][y] = typeBloc;
                        nombreItemsDeux[x][y]++;

                        if (inventaire[x][y] == HERBE)
                        {
                            quantiteMaxItems[x][y] = MAX_HERBE;
                        }
                        else if (inventaire[x][y] == TERRE)
                        {
                            quantiteMaxItems[x][y] = MAX_TERRE;
                        }
                        else if (inventaire[x][y] == PIERRE)
                        {
                            quantiteMaxItems[x][y] = MAX_PIERRE;
                        }
                        else if (inventaire[x][y] == CHARBON)
                        {
                            quantiteMaxItems[x][y] = MAX_CHARBON;
                        }
                        else if (inventaire[x][y] == FER)
                        {
                            quantiteMaxItems[x][y] = MAX_FER;
                        }
                        else if (inventaire[x][y] == BOIS)
                        {
                            quantiteMaxItems[x][y] = MAX_BOIS;
                        }
                        else if (inventaire[x][y] == BOIS_NATUREL)
                        {
                            quantiteMaxItems[x][y] = MAX_BOIS_NATUREL;
                        }
                        else if (inventaire[x][y] == FEUILLE)
                        {
                            quantiteMaxItems[x][y] = MAX_FEUILLE;
                        }

                        trouver = true;
                        break;
                    }
                }

                if (trouver)
                {
                    break;
                }
            }
        }
    }
    else if (typeBloc != AIR)
    {
        for (int x(0); x < 8; x++)
        {
            for (int y(0); y < 10; y++)
            {
                if (inventaire[x][y] == typeBloc)
                {
                    if (nombreItemsDeux[x][y] - 1 == 0)
                    {
                        inventaire[x][y] = AIR;
                        nombreItemsDeux[x][y] = 0;
                        selectionInventaire[selectionBloc - 1] = AIR;
                    }
                    else
                    {
                        nombreItemsDeux[x][y]--;
                    }

                    trouver = true;
                    *ok = true;
                    break;
                }
            }

            if (trouver)
            {
                break;
            }
        }

        if (!trouver)
        {
            *ok = false;
        }
    }
}
