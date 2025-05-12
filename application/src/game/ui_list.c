/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/ui_list.h"
#include "game/game_common.h"
#include "game/scene.h"
#include "core/utils.h"

UIList *UIList_create(
    Scene *scene, SDL_FRect rect, float labelRatio, TTF_Font *font,
    const char *title, const char **values, int valueCount,
    SDL_Color textColor, SDL_Color backgroundColor, SDL_Color selectedColor)
{
    UIList *self = (UIList *)calloc(1, sizeof(UIList));
    AssertNew(self);

    self->m_scene = scene;
    self->m_valueCount = valueCount;
    self->m_values = (char **)calloc(valueCount, sizeof(char *));
    AssertNew(self->m_values);
    for (int i = 0; i < valueCount; i++)
    {
        self->m_values[i] = Strdup(values[i]);
    }

    self->m_buttonPrev = UIButton_create(
        scene, rect, font, "<", textColor, backgroundColor, selectedColor);
    self->m_buttonNext = UIButton_create(
        scene, rect, font, ">", textColor, backgroundColor, selectedColor);

    self->m_textTitle = Text_create(g_renderer, font, title, textColor);
    self->m_textValue = Text_create(g_renderer, font, values[0], textColor);

    UIList_setRect(self, rect, labelRatio);

    return self;
}

void UIList_setRect(UIList *self, SDL_FRect rect, float labelRatio)
{
    self->m_rect = rect;
    self->m_labelRatio = labelRatio;

    SDL_FRect dstRect = rect;
    dstRect.w = dstRect.h;
    dstRect.x = rect.x + labelRatio * rect.w;
    UIButton_setRect(self->m_buttonPrev, dstRect);
    dstRect.x = rect.x + rect.w - dstRect.w;
    UIButton_setRect(self->m_buttonNext, dstRect);
}

void UIList_destroy(UIList *self)
{
    if (!self) return;

    if (self->m_values)
    {
        for (int i = 0; i < self->m_valueCount; i++)
        {
            free(self->m_values[i]);
        }
        free(self->m_values);
    }
    Text_destroy(self->m_textTitle);
    Text_destroy(self->m_textValue);
    UIButton_destroy(self->m_buttonPrev);
    UIButton_destroy(self->m_buttonNext);
    free(self);
}

void UIList_render(UIList *self)
{
    assert(self && "The UIButton must be created");

    UIButton_render(self->m_buttonPrev);
    UIButton_render(self->m_buttonNext);

    SDL_FRect dstRect = { 0 };
    SDL_Texture *texture = Text_getTexture(self->m_textTitle);
    float w = (float)Text_getWidth(self->m_textTitle);
    float h = (float)Text_getHeight(self->m_textTitle);
    dstRect.x = roundf(self->m_rect.x);
    dstRect.y = roundf(self->m_rect.y + 0.5f * (self->m_rect.h - h));
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderTexture(g_renderer, texture, NULL, &dstRect);

    texture = Text_getTexture(self->m_textValue);
    w = (float)Text_getWidth(self->m_textValue);
    h = (float)Text_getHeight(self->m_textValue);
    dstRect.x = roundf(
        self->m_rect.x + self->m_labelRatio * self->m_rect.w 
        + 0.5f * ((1.f - self->m_labelRatio) * self->m_rect.w - w)
    );
    dstRect.y = roundf(self->m_rect.y + 0.5f * (self->m_rect.h - h));
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderTexture(g_renderer, texture, NULL, &dstRect);
}

void UIList_update(UIList *self)
{
    assert(self && "The UIButton must be created");
    UIButton_update(self->m_buttonPrev);
    UIButton_update(self->m_buttonNext);

    if (UIButton_isPressed(self->m_buttonPrev))
    {
        self->m_valueID = Int_clamp(self->m_valueID - 1, 0, self->m_valueCount - 1);
    }
    if (UIButton_isPressed(self->m_buttonNext))
    {
        self->m_valueID = Int_clamp(self->m_valueID + 1, 0, self->m_valueCount - 1);
    }

    bool prevEnabled = (self->m_valueID > 0);
    bool nextEnabled = (self->m_valueID < self->m_valueCount - 1);
    UIButton_setEnabled(self->m_buttonPrev, prevEnabled);
    UIButton_setEnabled(self->m_buttonNext, nextEnabled);

    Text_setString(self->m_textValue, self->m_values[self->m_valueID]);
}
