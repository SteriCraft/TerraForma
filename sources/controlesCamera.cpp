/*
         ### - PROJET GAME / controlesCamera.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 05/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include <vector>
#include "controlesCamera.h"
#include "environnement.h"
#include "joueur.h"
#include "majClavier.h"
#include "interface.h"
#include "menuMort.h"
#include "mobs.h"
#include "contactsEntitees.h"

static SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));

void controlesCamera(SDL_Surface *ecran, int modeJeu, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE], TTF_Font *policeTexte)
{
    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    SDL_Rect positionCurseur;

    int tempsActuel(0), tempsPrecedent(0), posPersoX(0), posPersoY(0), tempsPersoPrecedent(0), tempsPersoActuel(0), tempsBlocPrecedent(0), tempsBlocActuel(0);
    bool inventaire(false);

    Camera camera; // Initialisation de la caméra

    Joueur Kevin(IMG_Load("textures/skins/kevin_gauche.png"), world, &camera, largeurFenetre, hauteurFenetre); // Chargement de la texture du joueur (dossier: //textures/)

    std::vector<Troll*> listeTrolls(1);
    listeTrolls[listeTrolls.size() - 1] = new Troll(IMG_Load("textures/mobs/troll.png"), world, &camera, largeurFenetre, hauteurFenetre);

    listeTrolls[0]->recupererPosition(&posPersoX, &posPersoY);
    listeTrolls[0]->changerPosX((posPersoX / TAILLE_BLOCK) - 2);
    listeTrolls[0]->changerPosY((posPersoY / TAILLE_BLOCK) - 2);

    InterfaceJeu interface(largeurFenetre, modeJeu);

    while (!in.key[SDLK_ESCAPE]) // Boucle principale
    {
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent >= 16)
        {
            in.quit = false;
            majClavier(&in);

            if (in.quit)
            {
                break;
            }

            /* La pression des touches Q/D appelle des fonctions différentes selon le mode de jeu.
            Par exemple, la pression de la touche W en mode de jeu 1 (libre) appelle la modification de la
            position de la caméra d'un facteur additionnel 2. La même touche en mode de jeu 0 (aventure) appelle
            la modification de la position du personnage selon des paramètres décimaux (double), qui modifient
            eux-mêmes la position de la caméra (suivi du personnage)*/

            if (!inventaire)
            {
                if (in.key[SDLK_d] && in.key[SDLK_SPACE]) // Déplacements simultanés
                {
                    if (modeJeu == 1) // Mode libre
                    {
                        camera.posCamY -= 10;
                    }
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, 0.1, camera, false, false);
                        camera = Kevin.deplacerPersonnage(world, 0, camera, false, true);
                    }
                }
                else if (in.key[SDLK_a] && in.key[SDLK_SPACE])
                {
                    if (modeJeu == 1) // Mode libre
                    {
                        camera.posCamY -= 10;
                    }
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, -0.1, camera, false, false);
                        camera = Kevin.deplacerPersonnage(world, 0, camera, false, true);
                    }
                }
                else if (in.key[SDLK_SPACE]) // Déplacements seuls
                {
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, 0, camera, false, true);
                    }
                }
                else if (in.key[SDLK_a])
                {
                    if (modeJeu == 1) // Mode libre
                    {
                        camera.posCamX -= 10;
                    }
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, -0.1, camera, false, false);
                    }
                }
                else if (in.key[SDLK_d])
                {
                    if (modeJeu == 1) // Mode libre
                    {
                        camera.posCamX += 10;
                    }
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, 0.1, camera, false, false);
                    }
                }
                else
                {
                    if (modeJeu == 0) // Mode aventure
                    {
                        camera = Kevin.deplacerPersonnage(world, 0, camera, false, false);
                    }
                }
            }

            if (in.boutonSouris[SDL_BUTTON_WHEELDOWN]) // Modification de la sélection de bloc
            {
                interface.changerPositionSelection(true, largeurFenetre);
            }
            else if (in.boutonSouris[SDL_BUTTON_WHEELUP])
            {
                interface.changerPositionSelection(false, largeurFenetre);
            }
            else if (in.boutonSouris[SDL_BUTTON_LEFT]) // Suppression d'un bloc
            {
                Kevin.recupererPosition(&posPersoX, &posPersoY);

                if (in.sourisX > largeurFenetre - 245 && in.sourisX < largeurFenetre - 213)
                {
                    if (in.sourisY > 29 && in.sourisY < 61)
                    {
                        inventaire = true;
                    }
                }

                if (in.sourisX > (largeurFenetre / 2) + 360 && in.sourisX < (largeurFenetre / 2) + 373)
                {
                    if (in.sourisY > (hauteurFenetre / 2) - 275 && in.sourisY < (hauteurFenetre / 2) - 265)
                    {
                        if (inventaire)
                        {
                            inventaire = false;
                        }
                    }
                }

                if (inventaire)
                {
                    interface.testClicInventaire(in.sourisX, in.sourisY);
                }

                if (modeJeu == 0)
                {
                    if (verifModifBloc(posPersoX, posPersoY, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY))
                    {
                        if (!inventaire)
                        {
                            modifierBloc(world, AIR, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, true, &interface);
                        }
                    }
                }
                else
                {
                    if (!inventaire)
                    {
                        modifierBloc(world, AIR, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, true, &interface);
                    }
                }
            }
            else if (in.boutonSouris[SDL_BUTTON_RIGHT]) // Pose d'un bloc
            {
                Kevin.recupererPosition(&posPersoX, &posPersoY);

                if (modeJeu == 0)
                {
                    if (verifModifBloc(posPersoX, posPersoY, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY))
                    {
                        if (!inventaire)
                        {
                            modifierBloc(world, interface.blocSelectionner(), in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, false, &interface);
                        }
                    }
                }
                else
                {
                    if (!inventaire)
                    {
                        modifierBloc(world, interface.blocSelectionner(), in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, false, &interface);
                    }
                }
            }

            tempsPersoActuel = SDL_GetTicks();
            tempsBlocActuel = SDL_GetTicks();

            if (Kevin.changerVie() <= 0 && modeJeu == 0)
            {
                menuMort(ecran, largeurFenetre, hauteurFenetre, policeTexte);
                break;
            }

            if (tempsPersoActuel - tempsPersoPrecedent >= 2000 && modeJeu == 0)
            {
                Kevin.recupererPosition(&posPersoX, &posPersoY);
                if(Kevin.changerVie()==100 || Kevin.changerFatigue() < 50)
                {
                    Kevin.changerFatigue(10);
                }
                if (contactPersoTroll(listeTrolls, posPersoX, posPersoY))
                {
                    Kevin.changerVie(-FORCE_TROLL);
                }

                    Kevin.changerVie(10);

                tempsPersoPrecedent = tempsPersoActuel;
            }

            if (tempsBlocActuel - tempsBlocPrecedent >= 500)
            {
                majTerre(world);

                tempsBlocPrecedent = tempsBlocActuel;
            }

            positionCurseur.x = in.sourisX;
            positionCurseur.y = in.sourisY;

            /* Mise à jour de l'écran de jeu selon un ordre précis:
            1- Affichage des blocs
            2- Affichage des textures
            3- Affichage des personnages
            4- Affichage des menus*/

            bliterArrierePlan(ecran, camera, largeurFenetre, hauteurFenetre, world);
            bliterEcran(ecran, world, camera, largeurFenetre, hauteurFenetre);

            if (modeJeu == 0) // Le personnage n'est afficher qu'en mode de jeu 0 (aventure)
            {
                if (!inventaire)
                {
                    camera = Kevin.deplacerPersonnage(world, 0, camera, true, false);
                }

                Kevin.afficherPersonnageCamera(ecran, largeurFenetre, hauteurFenetre);
            }
            else
            {
                if (in.key[SDLK_s] && !inventaire)
                {
                    camera.posCamY += 10;
                }
                else if (in.key[SDLK_w] && !inventaire)
                {
                    camera.posCamY -= 10;
                }
            }

            for (int mem(listeTrolls.size()); mem > 0; mem--)
            {
                listeTrolls[mem - 1]->deplacerPersonnage(world, 0, camera, true, false);
                listeTrolls[mem - 1]->afficherPersonnage(ecran, camera);
            }

            bliterArbres(ecran, world, camera, largeurFenetre, hauteurFenetre);

            if (!inventaire)
            {
                interface.afficherInterface(ecran, Kevin.changerVie(), Kevin.changerFatigue());
            }

            if (inventaire)
            {
                interface.afficherInventaire(ecran, largeurFenetre, hauteurFenetre);

                if (modeJeu == 0)
                {
                    interface.afficherCompteurBlocInventaire(largeurFenetre, hauteurFenetre, policeTexte, ecran);
                }
            }

            SDL_BlitSurface(curseur, NULL, ecran, &positionCurseur);

            SDL_Flip(ecran); // Mise à jour de l'écran toutes les 16 ms

            tempsPrecedent = tempsActuel;
        }
    }
}

bool verifModifBloc(double posPersoX, double posPersoY, double posSourisX, double posSourisY) // Test afin de savoir si le bloc pointé peut être modifié (distance et type)
{
    double distance(0);

    distance = sqrt(((posSourisX - posPersoX) * (posSourisX - posPersoX)) + ((posSourisY - posPersoY) * (posSourisY - posPersoY)));

    if (distance < 6 * TAILLE_BLOCK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
