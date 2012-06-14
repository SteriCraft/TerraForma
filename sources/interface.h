/*
          ### - PROJET GAME / interface.h - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 05/06/2012
*/

#ifndef DEF_INTERFACE_H
#define DEF_INTERFACE_H

#include <SDL_ttf.h>

#define NOMBRE_ITEMS 8

class InterfaceJeu
{
    public:
        InterfaceJeu(int largeurFenetre, int modeJeu);
        ~InterfaceJeu();
        void afficherInterface(SDL_Surface *ecran, int viePerso, int fatiguePerso);
        void chargerTexturesItems();
        int blocSelectionner();
        void changerPositionSelection(bool direction, int largeurFenetre);
        void afficherInventaire(SDL_Surface *ecran, int largeurFenetre, int hauteurFenetre);
        void testClicInventaire(int posX, int posY);
        void afficherCompteurBlocInventaire(int largeurFenetre, int hauteurFenetre, TTF_Font *police, SDL_Surface *ecran);
        void ajouterEnleverBlocInventaire(int typeBloc, bool modifier, bool *ok);

    private:
        SDL_Surface *barreInventaire;
        SDL_Surface *indicateursPerso;
        SDL_Surface *tetePerso;
        SDL_Surface *barreVieUn;
        SDL_Surface *barreVieDeux;
        SDL_Surface *barreVieTrois;
        SDL_Surface *barreVieQuatre;
        SDL_Surface *barreVieCinq;
        SDL_Surface *barreFatigueUn;
        SDL_Surface *barreFatigueDeux;
        SDL_Surface *barreFatigueTrois;
        SDL_Surface *barreFatigueQuatre;
        SDL_Surface *barreFatigueCinq;
        SDL_Surface *iconeInventaire;
        SDL_Surface *selection;
        SDL_Surface *croix;
        SDL_Surface *items[NOMBRE_ITEMS];
        SDL_Surface *imageInventaire;
        SDL_Surface *blocInventaire;
        SDL_Surface *itemAir;
        SDL_Surface *itemHerbe;
        SDL_Surface *itemTerre;
        SDL_Surface *itemPierre;
        SDL_Surface *itemCharbon;
        SDL_Surface *itemFer;
        SDL_Surface *itemBois;
        SDL_Surface *itemBoisNaturel;
        SDL_Surface *itemFeuille;

        SDL_Rect positionItems[4];
        SDL_Rect positionBarreInventaire;
        SDL_Rect positionIndicateursPerso;
        SDL_Rect positionTetePerso;
        SDL_Rect positionBarreVie;
        SDL_Rect positionBarreFatigue;
        SDL_Rect positionIconeInventaire;
        SDL_Rect positionSelection;
        SDL_Rect positionInventaire;
        SDL_Rect positionBloc;
        SDL_Rect positionCroix;
        SDL_Rect positionNombreItems;

        int selectionBloc;
        int inventaire[8][10];
        int selectionInventaire[4];
        int nombreItemsDeux[8][10];
        int quantiteMaxItems[8][10];
};

#endif
