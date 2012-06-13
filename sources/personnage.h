/*
            ### - PROJET GAME / personnage.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 05/06/2012
*/

#ifndef DEF_PERSONNAGE_H
#define DEF_PERSONNAGE_H

#include <vector>
#include "controlesCamera.h"
#include "environnement.h"
//#include "mobs.h"

#define FACTEUR_DEGATS_CHUTE 250

class Personnage
{
    public:
        Personnage(SDL_Surface *texturePerso, Portion_Map world[][PROFONDEUR_MONDE], Camera *camera, int largeurFenetre, int hauteurFenetre, int viePerso = 200, int massePerso = 60.0);
        ~Personnage();
        void afficherPersonnageCamera(SDL_Surface *ecran, int largeurFenetre, int hauteurFenetre);
        void afficherPersonnage(SDL_Surface *ecran, Camera camera);
        Camera deplacerPersonnage(Portion_Map chunk[][PROFONDEUR_MONDE], double posX, Camera camera, bool gravity, bool saut);
        void changerTexture(SDL_Surface *nouvelleTexture);
        void detecterCollision(Portion_Map chunk[][PROFONDEUR_MONDE], bool *okSaut, bool *gravity);
        bool contact(Portion_Map chunk[][PROFONDEUR_MONDE], int positionPersoX, int positionPersoY);
        void sauter(bool saut, bool okSaut);
        void recupererPosition(int *posX, int *posY);
        int changerVie(int modifVie=0);
        int changerFatigue(int modifFatigue=0);
        void changerPosX(int posX);
        void changerPosY(int posY);
        void changerVitX(int vitX);
        void changerVitY(int vitY);
        void replacerPersonnage(Portion_Map chunk[][PROFONDEUR_MONDE]);
        void changerMasse(int poids);

    private:
        int armure;
        int vie;
        int fatigue;
        double masse;
        SDL_Surface *texture;
        SDL_Rect positionPerso;
        double coorX;
        double coorY;
        double vitesseX;
        double vitesseY;
};

#endif
