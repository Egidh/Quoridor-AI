/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/timer.h"

#define MIX_CHANNEL_COUNT 16
typedef struct AssetManager AssetManager;

/// @brief Temps global du jeu.
extern Timer *g_time;

/// @brief Fenêtre du jeu.
extern SDL_Window *g_window;

/// @brief Moteur de rendu du jeu.
extern SDL_Renderer *g_renderer;

/// @brief Initialise les librairies utilisées par le jeu.
/// @param sdlFlags les flags pour la librairie SDL.
/// @param mixFlags les flags pour la librairie SDL Mixer.
SDL_AppResult Game_init(int sdlFlags, int mixFlags);

/// @brief Crée la fenêtre du jeu.
/// @param width largeur de la fenêtre.
/// @param height hauteur de la fenêtre.
/// @param title la titre de la fenêtre.
/// @param flags les flags SDL.
SDL_AppResult Game_createWindow(int width, int height, const char *title, Uint32 flags);

/// @brief Crée le moteur de rendu.
/// @param width largeur logique du rendu.
/// @param height hauteur logique du rendu.
SDL_AppResult Game_createRenderer(int width, int height);

/// @brief Détruit le moteur de rendu du jeu.
void Game_destroyRenderer();

/// @brief Détruit la fenêtre du jeu.
void Game_destroyWindow();

/// @brief Quitte les librairies utilisées par le jeu.
void Game_quit();

/// @brief Définit la couleur du moteur de rendu utilisée par les opérations draw ou fill.
/// @param color la couleur (le paramètre alpha est ignoré).
/// @param alpha l'opacité (0 pour transparant, 255 pour opaque).
void Game_setRenderDrawColor(SDL_Color color, Uint8 alpha);

void Memcpy(void * const dst, size_t dstSize, const void *src, size_t srcSize);
char *Strdup(const char *src);
