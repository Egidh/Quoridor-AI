/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/text.h"
#include "game/ui_button.h"

typedef struct Scene Scene;

typedef struct UIList
{
    Scene *m_scene;
    SDL_FRect m_rect;
    UIButton *m_buttonNext;
    UIButton *m_buttonPrev;
    char **m_values;
    int m_valueCount;
    int m_valueID;
    Text *m_textTitle;
    Text *m_textValue;
    float m_labelRatio;
} UIList;

UIList *UIList_create(
    Scene *scene, SDL_FRect rect, float labelRatio, TTF_Font *font,
    const char *title, const char **values, int valueCount,
    SDL_Color textColor, SDL_Color backgroundColor, SDL_Color selectedColor);

void UIList_setRect(UIList *self, SDL_FRect rect, float labelRatio);

void UIList_destroy(UIList *self);
void UIList_render(UIList *self);
void UIList_update(UIList *self);

INLINE int UIList_getSelected(UIList *self)
{
    assert(self && "The UIList must be created");
    return self->m_valueID;
}

INLINE void UIList_setSelected(UIList *self, int valueID)
{
    assert(self && "The UIList must be created");
    assert(0 <= valueID && valueID < self->m_valueCount);
    self->m_valueID = valueID;
}
