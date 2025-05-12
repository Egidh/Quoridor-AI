/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "core/quoridor_core.h"
#include "core/quoridor_ai.h"
#include "engine/text.h"
#include "game/ui_button.h"
#include "game/ui_list.h"

typedef struct Scene Scene;

typedef struct UIQuoridor
{
    Scene *m_scene;

    SDL_FRect m_rectCells[MAX_GRID_SIZE][MAX_GRID_SIZE];
    SDL_FRect m_rectHWalls[MAX_GRID_SIZE - 1][MAX_GRID_SIZE - 1];
    SDL_FRect m_rectVWalls[MAX_GRID_SIZE - 1][MAX_GRID_SIZE - 1];
    SDL_FRect m_rectMouseHWalls[MAX_GRID_SIZE - 1][MAX_GRID_SIZE - 1];
    SDL_FRect m_rectMouseVWalls[MAX_GRID_SIZE - 1][MAX_GRID_SIZE - 1];
    SDL_FRect m_rectSettings;
    SDL_FRect m_rectMainPanels;

    Text *m_textTitleInfo;
    Text *m_textInfo;
    Text *m_textTitleWalls;
    Text *m_textWalls[2];
    Text *m_textTitleDistances;
    Text *m_textDistances[2];
    Text *m_textTitleSettings;

    UIButton *m_buttonSettings;
    UIButton *m_buttonRestart;
    UIButton *m_buttonBack;

    UIList *m_listMode;
    UIList *m_listLevel;
    UIList *m_listGridSize;
    UIList *m_listCPUTime;
    UIList *m_listWallCount;
    UIList *m_listRandStart;

    bool m_inSettings;

    QuoridorTurn m_aiTurn;
    void *m_aiData[2];

    Uint64 m_aiAccu;
} UIQuoridor;

UIQuoridor *UIQuoridor_create(Scene *scene);
void UIQuoridor_destroy(UIQuoridor *self);
void UIQuoridor_update(UIQuoridor *self);
void UIQuoridor_updateRects(UIQuoridor *self);
void UIQuoridor_updatePageMain(UIQuoridor *self);
void UIQuoridor_updatePageSettings(UIQuoridor *self);
void UIQuoridor_updateTurn(UIQuoridor *self);

void UIQuoridor_restartQuoridor(UIQuoridor *self);

void UIQuoridor_render(UIQuoridor *self);
void UIQuoridor_renderPageMain(UIQuoridor *self);
void UIQuoridor_renderPageSettings(UIQuoridor *self);
void UIQuoridor_renderBoard(UIQuoridor *self);
