/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "settings.h"
#include "engine/common.h"
#include "engine/camera.h"
#include "engine/text.h"

#include "game/game_common.h"
#include "game/input.h"
#include "game/scene.h"

#include <SDL3/SDL_main.h>

//#define FULLSCREEN
//#define WINDOW_FHD

#ifdef WINDOW_FHD
#define WINDOW_WIDTH   FHD_WIDTH
#define WINDOW_HEIGHT  FHD_HEIGHT
#else
#define WINDOW_WIDTH   HD_WIDTH
#define WINDOW_HEIGHT  HD_HEIGHT
#endif
#define LOGICAL_WIDTH  FHD_WIDTH
#define LOGICAL_HEIGHT FHD_HEIGHT

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    Scene *scene = NULL;
    bool quitGame = false;

    //--------------------------------------------------------------------------
    // Initialisation

    // Initialisation de la SDL
    const Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_GAMEPAD;
    const Uint32 mixFlags = 0;// MIX_INIT_MP3;
    Game_init(sdlFlags, mixFlags);

    // Crée la fenêtre et le moteur de rendu
    Uint32 windowFlags = SDL_WINDOW_RESIZABLE;
#ifdef FULLSCREEN
    windowFlags |= SDL_WINDOW_FULLSCREEN;
#endif
    Game_createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, u8"Quoridor", windowFlags);
    Game_createRenderer(LOGICAL_WIDTH, LOGICAL_HEIGHT);

    //--------------------------------------------------------------------------
    // Boucle de jeu

    // Paramètres
    GameConfig gameConfig = { 0 };
    gameConfig.nextScene = GAME_SCENE_MAIN;
    bool drawGizmos = false;
    
    while (quitGame == false)
    {
        switch (gameConfig.nextScene)
        {
        case GAME_SCENE_MAIN:
            scene = Scene_create(&gameConfig);
            Scene_mainLoop(scene, drawGizmos);
            Scene_destroy(scene);
            scene = NULL;

        case GAME_SCENE_QUIT:
        default:
            quitGame = true;
            break;
        }
    }

    //--------------------------------------------------------------------------
    // Libération de la mémoire

    Scene_destroy(scene); scene = NULL;
    Game_destroyRenderer();
    Game_destroyWindow();
    Game_quit();

    return EXIT_SUCCESS;
}