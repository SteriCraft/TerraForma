/*
          ### - PROJET GAME / personnage.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 12/06/2012
*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include "personnage.h"
#include "environnement.h"
#include "controlesCamera.h"
#include "tableParametresBloc.h"

Personnage::Personnage(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre, int viePerso, int massePerso) : vie(viePerso), masse(massePerso), texture(texturePerso) // Constructeur de la classe Personnage, 100 points de vie, 2 points de dégâts infligeables, pas d'expérience
{
    kevinDroit = IMG_Load("textures/skins/kevin_droit.png");
    kevinGauche = IMG_Load("textures/skins/kevin_gauche.png");

    int chunkX(0), chunkY(0), blocX(0), blocY(0), memX(0), memY(0); // Variables de travail
    double x(0);

    x = LARGEUR_MONDE;
    coorX = (x / 2) * LARGEUR_PARTIE_MAP; // Initialisation des variables de travail (localisation de l'emplacement du personnage)
    coorY = 0;
    armure = 0;
    fatigue = 100;
    memX = coorX + (texture->w / TAILLE_BLOCK);
    memY = coorY + (texture->h / TAILLE_BLOCK);
    chunkX = memX / LARGEUR_PARTIE_MAP;
    chunkY = memY / LARGEUR_PARTIE_MAP;
    blocX = memX % LARGEUR_PARTIE_MAP;
    blocY = memY % LARGEUR_PARTIE_MAP;

    while (world[chunkX][chunkY].blocs[blocX][blocY].type == AIR) // Positionnement initial du personnage sur le sol du jeu
    {
        coorY++;
        memY = coorY + (texture->h / TAILLE_BLOCK);
        chunkY = coorY / LARGEUR_PARTIE_MAP;
        blocY = memY % LARGEUR_PARTIE_MAP;
    }

    camera->posCamX = (coorX * TAILLE_BLOCK) - ((largeurFenetre / 2) - (texture->w / 2));
    camera->posCamY = (coorY * TAILLE_BLOCK) - ((hauteurFenetre / 2) - (texture->h / 2));
}

Personnage::~Personnage()
{
    SDL_FreeSurface(kevinGauche); // Libération des surfaces pour éviter les fuites mémoires
    SDL_FreeSurface(kevinDroit);
    SDL_FreeSurface(texture);
}

void Personnage::afficherPersonnageCamera(SDL_Surface *ecran, int largeurFenetre, int hauteurFenetre)
{
    positionPerso.x = (largeurFenetre / 2) - (texture->w / 2); // Positionnement du personnage sur l'écran
    positionPerso.y = (hauteurFenetre / 2) - (texture->h / 2) - 7;

    SDL_BlitSurface(texture, NULL, ecran, &positionPerso); // Affichage du personnage
}

void Personnage::afficherPersonnage(SDL_Surface *ecran, Camera camera)
{
    positionPerso.x = (coorX * TAILLE_BLOCK) - camera.posCamX; // Positionnement du personnage sur l'écran
    positionPerso.y = (coorY * TAILLE_BLOCK) - camera.posCamY;

    SDL_BlitSurface(texture, NULL, ecran, &positionPerso); // Affichage du personnage
}

Camera Personnage::deplacerPersonnage(Portion_Map chunk[][PROFONDEUR_MONDE], double accelerationX, Camera camera, bool gravity, bool saut) // Fonction de déplacement du personnage
{
    bool okSaut(false), gravityActive(true), stop(false);
    int posTestX(0), posTestY(0);

    vitesseX = accelerationX;

    detecterCollision(chunk, &okSaut, &gravityActive); // Test de collision avec un/des bloc(s)

    if (gravityActive && gravity)
    {
        vitesseY += 0.01; // Gravité
    }

    sauter(saut, okSaut); // Saut

    posTestX = (coorX + vitesseX) * 16;
    posTestY = (coorY + vitesseY) * 16;

    if (posTestX > LARGEUR_MONDE * (LARGEUR_PARTIE_MAP - 1) * TAILLE_BLOCK || posTestY > PROFONDEUR_MONDE * (PROFONDEUR_PARTIE_MAP - 1) * TAILLE_BLOCK || posTestX < 0 || posTestY < 0) // Test de position (murs invisibles)
    {
        stop = true;
    }
    else
    {
        stop = false;
    }

    if (!stop)
    {
        coorX += vitesseX;
        camera.posCamX += (vitesseX * TAILLE_BLOCK);

        coorY += vitesseY;
        camera.posCamY += (vitesseY * TAILLE_BLOCK);
    }

    if (vitesseX > 0)
    {
        changerTexture(kevinDroit);
    }
    else if (vitesseX < 0)
    {
        changerTexture(kevinGauche);
    }

    return camera; // L'état de la caméra est mis à jour
}

void Personnage::changerTexture(SDL_Surface *nouvelleTexture) // Changement de texture (inGame, pour les animations)
{
    texture = nouvelleTexture;
}

void Personnage::detecterCollision(Portion_Map chunk[][PROFONDEUR_MONDE], bool *okSaut, bool *gravity) // Fonctions de détection des collisions
{
    int positionPersoX((coorX * TAILLE_BLOCK) + (texture->w / 4)), positionPersoY(((coorY * TAILLE_BLOCK) + texture->h) - 5);
    bool collide(false), touch(false);

    collide = contact(chunk, positionPersoX, positionPersoY); // Détection avancée... (10 points de contacts)

    if (collide) // Réactions si collision...
    {
        *gravity = false;
        *okSaut = true;

        if (!touch && vitesseY > 0.3 && vitesseY <= 0.32)
        {
            changerVie(-15);
            changerFatigue(-15);
            touch = true;
        }
        else if (!touch && vitesseY > 0.32 && vitesseY <= 0.34)
        {
            changerVie(-30);
            changerFatigue(-30);
            touch = true;
        }
        else if (!touch && vitesseY > 0.34 && vitesseY <= 0.36)
        {
            changerVie(-50);
            changerFatigue(-50);
            touch = true;
        }
        else if (!touch && vitesseY > 0.36 && vitesseY <= 0.4)
        {
            changerVie(-80);
            changerFatigue(-80);
            touch = true;
        }
        else if (!touch && vitesseY > 0.4)
        {
            changerVie(-100);
            changerFatigue(-100);
            touch = true;
        }

        vitesseY = 0;
    }
    else
    {
        *okSaut = false;
    }

    positionPersoX = (coorX * TAILLE_BLOCK) + ((texture->w / 4) * 3);

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide) // Réactions si collision...
    {
        *gravity = false;
        *okSaut = true;

        if (!touch && vitesseY > 0.3 && vitesseY <= 0.32)
        {
            changerVie(-15);
            changerFatigue(-15);
            touch = true;
        }
        else if (!touch && vitesseY > 0.32 && vitesseY <= 0.34)
        {
            changerVie(-30);
            changerFatigue(-30);
            touch = true;
        }
        else if (!touch && vitesseY > 0.34 && vitesseY <= 0.36)
        {
            changerVie(-50);
            changerFatigue(-50);
            touch = true;
        }
        else if (!touch && vitesseY > 0.36 && vitesseY <= 0.4)
        {
            changerVie(-80);
            changerFatigue(-80);
            touch = true;
        }
        else if (!touch && vitesseY > 0.4)
        {
            changerVie(-100);
            changerFatigue(-100);
            touch = true;
        }



        vitesseY = 0;
    }
    else
    {
        if (!(*okSaut))
        {
            *okSaut = false;
        }
    }

    //

    positionPersoX = (coorX * TAILLE_BLOCK) + (texture->w / 8);
    positionPersoY = (coorY * TAILLE_BLOCK) - 5;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        *gravity = true;
        *okSaut = false;
        vitesseY = 0;
    }

    positionPersoX = (coorX * TAILLE_BLOCK) + ((texture->w / 8) * 7);

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        *gravity = true;
        *okSaut = false;
        vitesseY = 0;
    }

    //

    positionPersoX = coorX * TAILLE_BLOCK;
    positionPersoY = (coorY * TAILLE_BLOCK) + ((texture->h / 8) * 7);

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }

    positionPersoX = (coorX * TAILLE_BLOCK) + texture->w;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }

    //

    positionPersoX = coorX * TAILLE_BLOCK;
    positionPersoY = (coorY * TAILLE_BLOCK) + ((texture->h / 8) * 5);

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }


    positionPersoX = (coorX * TAILLE_BLOCK) + texture->w;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }

    //

    positionPersoX = coorX * TAILLE_BLOCK;
    positionPersoY = (coorY * TAILLE_BLOCK) + (texture->h / 8);

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }

    positionPersoX = (coorX * TAILLE_BLOCK) + texture->w;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }

    //

    positionPersoX = coorX * TAILLE_BLOCK;
    positionPersoY = coorY * TAILLE_BLOCK;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }


    positionPersoX = (coorX * TAILLE_BLOCK) + texture->w;

    collide = contact(chunk, positionPersoX, positionPersoY);

    if (collide)
    {
        if (contact(chunk, (positionPersoX + (vitesseX * TAILLE_BLOCK)), (positionPersoY + (vitesseY * TAILLE_BLOCK))))
        {
            vitesseX = 0;
        }
    }
}

bool Personnage::contact(Portion_Map chunk[][PROFONDEUR_MONDE], int positionPersoX, int positionPersoY) // Fonction de détection avancée de collisions
{
    bool collide(false);
    int typeBloc(0), chunkX(0), chunkY(0);
    double memX(0), memY(0);

    chunkX = (positionPersoX / TAILLE_BLOCK) / LARGEUR_PARTIE_MAP;
    chunkY = (positionPersoY / TAILLE_BLOCK) / PROFONDEUR_PARTIE_MAP;

    for (int a(0); a < LARGEUR_PARTIE_MAP; a++)
    {
        for (int b(0); b < PROFONDEUR_PARTIE_MAP; b++)
        {
            memX = chunk[chunkX][chunkY].posX + (a * TAILLE_BLOCK);
            memY = chunk[chunkX][chunkY].posY + (b * TAILLE_BLOCK);

            if (positionPersoX > memX && (positionPersoX < memX + TAILLE_BLOCK) && positionPersoY > memY && (positionPersoY < memY + TAILLE_BLOCK))
            {
                typeBloc = chunk[chunkX][chunkY].blocs[a][b].type;
                break;
            }
        }
    }

    if (testBlocSolide(typeBloc))
    {
        collide = true;
    }

    return collide;
}

void Personnage::sauter(bool saut, bool okSaut) // Fonction de saut (en fonction de la masse)
{
    if (saut && okSaut && fatigue > 0)
    {
        vitesseY = -7.0 / masse;
        changerFatigue(-5);
    }
}

void Personnage::recupererPosition(int *posX, int *posY) // Renvoi de la position du personnage au pixel près
{
    *posX = (coorX * TAILLE_BLOCK) + (texture->w / 2);
    *posY = (coorY * TAILLE_BLOCK) + (texture->h / 2);
}

int Personnage::changerVie(int modifVie) // Fonction par Babanar
{
    if(modifVie>0) // si c'est un gain de vie alors...
    {
        vie += modifVie; // rajouter le modificateur de vie à la vie
        if (vie >100) // si la vie après la modification est suppérieur à 100...
        {
            vie = 100; // la vie est égal à 100
        }
    }
    else if (modifVie<0) // si c'est une perte de vie alors...
    {
        armure += modifVie;// soustraire la vie perdue à l'armure
        if(armure<0) //et si il n'y a plus d'armure après cette perte d'armure
        {
            vie += armure;// soustraire les points non absorbé par l'armure à la vie
            armure = 0;//et remettre l'armure à 0
        }
        if (vie<0) // si la vie après la modification est inférieur à 0
        {
            vie = 0; // la vie est égal à 0
        }
    }
    return vie;
}

int Personnage::changerFatigue(int modifFatigue) // Fonction par Babanar
{
    if(modifFatigue>0) // si c'est un gain de fatigue alors...
    {
        fatigue += modifFatigue; // rajouter le modificateur de fatigue à la fatigue
        if (fatigue >100) // si la fatigue après la modification est suppérieur à 100...
        {
            fatigue = 100; // la fatigue est égal à 100
        }
    }
    else if (modifFatigue<0) // si c'est une perte de fatigue alors...
    {
        fatigue += modifFatigue;// soustraire la fatigue perdue à l'armure

        if (fatigue<0) // si la fatigue après la modification est inférieur à 0
        {
            fatigue = 0; // la fatigue est égal à 0
        }
    }
    return fatigue;
}


void Personnage::changerPosX(int posX)
{
    coorX = posX;
}

void Personnage::changerPosY(int posY)
{
    coorY = posY;
}

void Personnage::changerVitX(int vitX)
{
    vitesseX = vitX;
}

void Personnage::changerVitY(int vitY)
{
    vitesseY = vitY;
}

void Personnage::replacerPersonnage(Portion_Map chunk[][PROFONDEUR_MONDE])
{
    int chunkX(0), chunkY(0), blocX(0), blocY(0), memX(0), memY(0); // Variables de travail

    coorX = (LARGEUR_MONDE / 2) * LARGEUR_PARTIE_MAP; // Initialisation des variables de travail (localisation de l'emplacement du personnage)
    coorY = 0;
    memX = coorX + (texture->w / TAILLE_BLOCK);
    memY = coorY + (texture->h / TAILLE_BLOCK);
    chunkX = memX / LARGEUR_PARTIE_MAP;
    chunkY = memY / LARGEUR_PARTIE_MAP;
    blocX = memX % LARGEUR_PARTIE_MAP;
    blocY = memY % LARGEUR_PARTIE_MAP;

    while (chunk[chunkX][chunkY].blocs[blocX][blocY].type == AIR) // Positionnement initial du personnage sur le sol du jeu
    {
        coorY++;
        memY = coorY + (texture->h / TAILLE_BLOCK);
        chunkY = coorY / LARGEUR_PARTIE_MAP;
        blocY = memY % LARGEUR_PARTIE_MAP;
    }
}

void Personnage::changerMasse(int poids)
{
    masse = poids;
}
