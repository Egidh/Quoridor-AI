/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/asset_manager.h"

//#define DEPLOY

#ifndef DEPLOY
#  define ASSETS_PATH "../../assets/"
#else
#  define ASSETS_PATH "./assets/"
#endif

#define MAX_PLAYER_COUNT 2

typedef enum GameScene
{
    GAME_SCENE_MAIN,
    GAME_SCENE_QUIT
} GameScene;

typedef struct GameConfig
{
    int nextScene;
} GameConfig;

typedef enum SceneState
{
    SCENE_STATE_RUNNING,
    SCENE_STATE_FADING_IN,
    SCENE_STATE_FADING_OUT,
    SCENE_STATE_FINISHED
} SceneState;

typedef struct GameColors
{
    SDL_Color black;
    SDL_Color white;
    SDL_Color red;
    SDL_Color blue;
    SDL_Color green;
    SDL_Color yellow;
    SDL_Color magenta;
    SDL_Color cyan;
    //
    SDL_Color gold;
    SDL_Color whiteSemi;
    SDL_Color wall;
    SDL_Color back;
    SDL_Color cell;
    SDL_Color player0;
    SDL_Color player1;
    SDL_Color move0;
    SDL_Color move1;
    SDL_Color selected;
} GameColors;

extern GameColors g_colors;

typedef enum SpriteID
{
    SPRITE_COUNT,
} SpriteID;

typedef enum FontID
{
    FONT_NORMAL,
    FONT_BIG,
    //
    FONT_COUNT,
} FontID;

void Game_addAssets(AssetManager *assets);
