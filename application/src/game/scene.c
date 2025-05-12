/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/scene.h"

Scene *Scene_create(GameConfig *gameConfig)
{
    Scene *self = (Scene *)calloc(1, sizeof(Scene));
    AssertNew(self);

    self->m_assets = AssetManager_create(SPRITE_COUNT, FONT_COUNT);
    Game_addAssets(self->m_assets);

    self->m_input = Input_create();

    int outW = 0, outH = 0;
    SDL_GetRenderOutputSize(g_renderer, &outW, &outH);

    self->m_camera = Camera_create(outW, outH);
    self->m_state = SCENE_STATE_FADING_IN;
    self->m_fadingTime = 0.5f;
    self->m_gameConfig = gameConfig;
    self->m_gameConfig->nextScene = GAME_SCENE_QUIT;

    self->m_core = QuoridorCore_create();
    self->m_quoridorUI = UIQuoridor_create(self);

    return self;
}

void Scene_destroy(Scene *self)
{
    if (!self) return;

    AssetManager_destroy(self->m_assets);
    Input_destroy(self->m_input);
    Camera_destroy(self->m_camera);
    UIQuoridor_destroy(self->m_quoridorUI);
    QuoridorCore_destroy(self->m_core);

    free(self);
}

void Scene_mainLoop(Scene *self, bool drawGizmos)
{
    assert(self && "The Scene must be created");
    Timer_setTimeScale(g_time, 1.f);
    while (true)
    {
        // Met à jour le temps
        Timer_update(g_time);

        Scene_update(self);

        Input *input = Scene_getInput(self);
        if (input->quitPressed)
        {
            self->m_gameConfig->nextScene = GAME_SCENE_QUIT;
            return;
        }

        if (self->m_state == SCENE_STATE_FINISHED)
            return;

        // Rend la scène
        Scene_render(self);

        if (drawGizmos) Scene_drawGizmos(self);

        // Affiche le nouveau rendu
        SDL_RenderPresent(g_renderer);
    }
}

void Scene_update(Scene *self)
{
    assert(self && "The Scene must be created");
    Input_update(self->m_input);

    UIQuoridor_update(self->m_quoridorUI);

    if (self->m_state == SCENE_STATE_FADING_IN)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_RUNNING;
        }
    }
    if (self->m_state == SCENE_STATE_FADING_OUT)
    {
        self->m_accu += Timer_getUnscaledDelta(g_time);
        if (self->m_accu >= self->m_fadingTime)
        {
            self->m_state = SCENE_STATE_FINISHED;
        }
    }
}

void Scene_quit(Scene *self)
{
    assert(self && "The Scene must be created");
    if ((self->m_state == SCENE_STATE_FADING_OUT) ||
        (self->m_state == SCENE_STATE_FINISHED))
        return;

    self->m_state = SCENE_STATE_FADING_OUT;
    self->m_accu = 0.f;
}

void Scene_render(Scene *self)
{
    assert(self && "The Scene must be created");

    // Efface le rendu précédent
    SDL_SetRenderDrawColor(g_renderer, 37, 37, 37, 255);
    SDL_RenderClear(g_renderer);

    Camera_updateViewport(self->m_camera, g_renderer);

    UIQuoridor_render(self->m_quoridorUI);

    // Fading
    if ((self->m_state == SCENE_STATE_FADING_IN) ||
        (self->m_state == SCENE_STATE_FADING_OUT))
    {
        int opacity = (int)(255.f * self->m_accu / self->m_fadingTime);
        if (self->m_state == SCENE_STATE_FADING_IN)
        {
            opacity = 255 - opacity;
        }
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, opacity);
        SDL_RenderFillRect(g_renderer, NULL);
    }
}

void Scene_drawGizmos(Scene *self)
{
    assert(self && "The Scene must be created");
}
