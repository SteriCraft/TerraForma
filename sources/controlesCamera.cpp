/*
         ### - PROJET GAME / controlesCamera.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 15/06/2012
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

void controlesCamera(SDL_Surface *ecran, int modeJeu, int largeurFenetre, int hauteurFenetre, Portion_Map world[][PROFONDEUR_MONDE], TTF_Font *policeTexte)
{
    SDL_Surface *curseur(IMG_Load("textures/interface/curseur.png"));

    int tempsJour(40000), tempsJourActuel(0), tempsJourPrecedent(0); // Le temps est fig� au matin pour les tests des fonctions de luminosit�

    ReceptionClavier in;
    memset(&in, 0, sizeof(in));

    SDL_Rect positionCurseur;

    int tempsActuel(0), tempsPrecedent(0), posPersoX(0), posPersoY(0), tempsPersoPrecedent(0), tempsPersoActuel(0), tempsBlocPrecedent(0), tempsBlocActuel(0);
    bool inventaire(false);

    Camera camera; // Initialisation de la cam�ra

    Joueur Kevin(IMG_Load("textures/skins/kevin_gauche.png"), world, &camera, largeurFenetre, hauteurFenetre); // Chargement de la texture du joueur (dossier: //textures/)

    std::vector<Troll*> listeTrolls(1);
    listeTrolls[listeTrolls.size() - 1] = new Troll(IMG_Load("textures/mobs/troll.png"), world, &camera, largeurFenetre, hauteurFenetre);

    listeTrolls[0]->recupererPosition(&posPersoX, &posPersoY);
    listeTrolls[0]->changerPosX((posPersoX / TAILLE_BLOCK) - 2);
    listeTrolls[0]->changerPosY((posPersoY / TAILLE_BLOCK) - 2);

    InterfaceJeu interface(largeurFenetre, modeJeu);

    if (modeJeu == 1) // En mode libre, le sac est automatiquement activ�, permettant de prendre plus de blocs. Cette partie est temporaire, tant que le nombre de blocs est inf�rieur � 80
    {
        interface.Sac(true);
    }

    //creerSourceLumiere(world, 80, 80, 3); Sources de lumi�re pour les tests
    //creerSourceLumiere(world, 80, 120, 6);

    while (!in.key[SDLK_ESCAPE]) // Boucle principale
    {
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent >= 16)
        {
            in.quit = false;
            majClavier(&in, true);

            if (in.quit)
            {
                break;
            }

            /* La pression des touches Q/D appelle des fonctions diff�rentes selon le mode de jeu.
            Par exemple, la pression de la touche W en mode de jeu 1 (libre) appelle la modification de la
            position de la cam�ra d'un facteur additionnel 2. La m�me touche en mode de jeu 0 (aventure) appelle
            la modification de la position du personnage selon des param�tres d�cimaux (double), qui modifient
            eux-m�mes la position de la cam�ra (suivi du personnage)*/

            if (!inventaire)
            {
                if (in.key[SDLK_d] && in.key[SDLK_SPACE]) // D�placements simultan�s
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
                else if (in.key[SDLK_SPACE]) // D�placements seuls
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

            if (in.boutonSouris[SDL_BUTTON_WHEELDOWN]) // Modification de la s�lection de bloc
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
                            modifierBloc(world, AIR, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, true, &interface, true);
                        }
                    }
                }
                else
                {
                    if (!inventaire)
                    {
                        modifierBloc(world, AIR, in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, true, &interface, false);
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
                            modifierBloc(world, interface.blocSelectionner(), in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, false, &interface, true);
                        }
                    }
                }
                else
                {
                    if (!inventaire)
                    {
                        modifierBloc(world, interface.blocSelectionner(), in.sourisX + camera.posCamX, in.sourisY + camera.posCamY, false, &interface, false);
                    }
                }
            }

            tempsPersoActuel = SDL_GetTicks();
            tempsBlocActuel = SDL_GetTicks();
            tempsJourActuel = SDL_GetTicks();

            if (Kevin.changerVie() <= 0 && modeJeu == 0)
            {
                menuMort(ecran, largeurFenetre, hauteurFenetre, policeTexte);
                break;
            }

            if (tempsPersoActuel - tempsPersoPrecedent >= 2000 && modeJeu == 0)
            {
                Kevin.recupererPosition(&posPersoX, &posPersoY);
                if(Kevin.changerVie() == 100 || Kevin.changerFatigue() < 50)
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

            if (tempsBlocActuel - tempsBlocPrecedent >= 1000)
            {
                majTerre(world);

                tempsBlocPrecedent = tempsBlocActuel;
            }

            /*if (tempsJourActuel - tempsJourPrecedent >= 42) // Ecoulement du temps, stopp� le temps que les fonctions de luminosit� soit termin�es
            {
                tempsJour += 10;

                tempsJourPrecedent = tempsJourActuel;
            }*/

            positionCurseur.x = in.sourisX;
            positionCurseur.y = in.sourisY;

            /* Mise � jour de l'�cran de jeu selon un ordre pr�cis:
            1- Affichage des blocs
            2- Affichage des textures
            3- Affichage des personnages
            4- Affichage des menus*/

            bliterArrierePlan(ecran, camera, largeurFenetre, hauteurFenetre, world, tempsJour);

            appliquerLumiere(world, camera, largeurFenetre, hauteurFenetre);

            bliterEcran(ecran, world, camera, largeurFenetre, hauteurFenetre);

            if (modeJeu == 0) // Le personnage n'est afficher qu'en mode de jeu 0 (aventure)
            {
                if (!inventaire)
                {
                    camera = Kevin.deplacerPersonnage(world, 0, camera, true, false); // Application de la gravit� sur le joueur, uniquement si l'inventaire est ferm�
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

            SDL_Flip(ecran); // Mise � jour de l'�cran toutes les 16 ms

            tempsPrecedent = tempsActuel;
        }
        else
        {
            SDL_Delay(16 - (tempsActuel - tempsPrecedent));
        }
    }

    for (unsigned int x(0); x < listeTrolls.size(); x++)
    {
        delete listeTrolls[x];
    }

    SDL_FreeSurface(curseur);
}

bool verifModifBloc(double posPersoX, double posPersoY, double posSourisX, double posSourisY) // Test afin de savoir si le bloc point� peut �tre modifi� (distance et type)
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
