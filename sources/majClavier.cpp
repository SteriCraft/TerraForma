/*
           ### - PROJET GAME / majClavier.cpp - ###

               Auteur: Gianni LADISA--LECLERCQ
      Date du fichier: 13/06/2012
*/

#include <SDL/SDL.h>
#include "majClavier.h"

void majClavier(ReceptionClavier *in) // Fonction de mise à jour du clavier (simplification de SDL_Event)
{
    SDL_Event event;

    in->boutonSouris[SDL_BUTTON_LEFT] = false;
    in->boutonSouris[SDL_BUTTON_RIGHT] = false;
    in->boutonSouris[SDL_BUTTON_WHEELUP] = false;
    in->boutonSouris[SDL_BUTTON_WHEELDOWN] = false;

	while(SDL_PollEvent(&event)) // Structure classique des events SDL
	{
		switch (event.type) // Boucle classique de la SDL
		{
		    case SDL_QUIT:
                in->quit = true;
                break;

            case SDL_KEYDOWN:
                in->key[event.key.keysym.sym] = true;
                break;

            case SDL_KEYUP:
                in->key[event.key.keysym.sym] = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                in->boutonSouris[event.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN)
                {
                    in->boutonSouris[event.button.button] = false;
                }
                break;

            case SDL_MOUSEMOTION:
                in->sourisX = event.motion.x;
                in->sourisY = event.motion.y;
                break;

            default:
                break;
		}
	}
}
