/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/game_common.h"
#include "engine/common.h"

GameColors g_colors = {
    .black = { 0, 0, 0, 255 },
    .white = { 255, 255, 255, 255 },
    .red = { 255, 0, 0, 255 },
    .green = { 0, 255, 0, 255 },
    .blue = { 0, 0, 255, 255 },
    .yellow = { 255, 255, 0, 255 },
    .cyan = { 0, 255, 255, 255 },
    .magenta = { 255, 0, 255, 255 },
    .gold = { 225, 185, 143, 255 },
    .whiteSemi = { 255, 255, 255, 127 },
    .wall = { 199, 100, 87, 255 },
    .back = { 14, 23, 27, 255 },
    .cell = { 27, 45, 54, 255},
    .selected = { 54, 79, 89, 255 },
    .player0 = { 63, 136, 197, 255},
    .player1 = { 231, 206, 116, 255},
    .move0 = { 129, 178, 217, 255},
    .move1 = { 240, 231, 168, 255},
};

void Game_addAssets(AssetManager *assets)
{
    // -------------------------------------------------------------------------
    // Fonts

    AssetManager_addFont(
        assets, FONT_NORMAL, ASSETS_PATH, "font/m5x7.dat", 28
    );
    AssetManager_addFont(
        assets, FONT_BIG, ASSETS_PATH, "font/futile_pro.dat", 48
    );
}
