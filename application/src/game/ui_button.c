/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/ui_button.h"
#include "game/game_common.h"
#include "game/scene.h"

UIButton *UIButton_create(
    Scene *scene, SDL_FRect rect, TTF_Font *font, const char *str,
    SDL_Color textColor, SDL_Color backgroundColor, SDL_Color selectedColor)
{
    UIButton *self = (UIButton *)calloc(1, sizeof(UIButton));
    AssertNew(self);

    self->m_scene = scene;
    self->m_label = Text_create(g_renderer, font, str, textColor);
    self->m_rect = rect;
    self->m_colorBackground = backgroundColor;
    self->m_colorSelected = selectedColor;
    self->m_state = UI_BUTTON_NORMAL;

    return self;
}

void UIButton_destroy(UIButton *self)
{
    if (!self) return;
    Text_destroy(self->m_label);
    free(self);
}

void UIButton_render(UIButton *self)
{
    assert(self && "The UIButton must be created");

    switch (self->m_state)
    {
    case UI_BUTTON_SELECTED: case UI_BUTTON_PRESSED:
        Game_setRenderDrawColor(self->m_colorSelected, 255);
        break;
    case UI_BUTTON_NORMAL:
        Game_setRenderDrawColor(self->m_colorBackground, 255);
        break;
    default:
        Game_setRenderDrawColor(self->m_colorBackground, 63);
        break;
    }
    SDL_RenderFillRect(g_renderer, &(self->m_rect));

    float w, h;
    Text_getSize(self->m_label, &w, &h);
    SDL_FRect dstRect = { 0 };
    SDL_Texture *texture = Text_getTexture(self->m_label);
    dstRect = self->m_rect;
    dstRect.x = roundf(dstRect.x + 0.5f * (dstRect.w - w));
    dstRect.y = roundf(dstRect.y + 0.5f * (dstRect.h - h));
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderTexture(g_renderer, texture, NULL, &dstRect);
}

void UIButton_update(UIButton *self)
{
    assert(self && "The UIButton must be created");

    if (self->m_state == UI_BUTTON_DISABLED) return;

    Input *input = Scene_getInput(self->m_scene);
    Vec2 mousePos = input->mousePos;

    self->m_state = UI_BUTTON_NORMAL;
    if (FRect_containsPoint(&(self->m_rect), mousePos))
    {
        self->m_state = (input->validatePressed) ? UI_BUTTON_PRESSED : UI_BUTTON_SELECTED;
    }
}

void UIButton_setEnabled(UIButton *self, bool isEnabled)
{
    assert(self && "The UIButton must be created");
    if (isEnabled)
    {
        if (self->m_state == UI_BUTTON_DISABLED)
            self->m_state = UI_BUTTON_NORMAL;
    }
    else
    {
        self->m_state = UI_BUTTON_DISABLED;
    }
}
