/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/text.h"

typedef struct Scene Scene;

typedef enum UIButtonState
{
    UI_BUTTON_NORMAL,
    UI_BUTTON_SELECTED,
    UI_BUTTON_PRESSED,
    UI_BUTTON_DISABLED
} UIButtonState;

typedef struct UIButton
{
    Scene *m_scene;
    int m_state;
    Text *m_label;
    SDL_FRect m_rect;
    SDL_Color m_colorBackground;
    SDL_Color m_colorSelected;
} UIButton;

UIButton *UIButton_create(
    Scene *scene, SDL_FRect rect, TTF_Font *font, const char *str,
    SDL_Color textColor, SDL_Color backgroundColor, SDL_Color selectedColor);
void UIButton_destroy(UIButton *self);
void UIButton_render(UIButton *self);
void UIButton_update(UIButton *self);
void UIButton_setEnabled(UIButton *self, bool isEnabled);

INLINE void UIButton_setRect(UIButton *self, SDL_FRect rect)
{
    assert(self && "The UIButton must be created");
    self->m_rect = rect;
}

INLINE void UIButton_setLabel(UIButton *self, const char *str)
{
    assert(self && "The UIButton must be created");
    Text_setString(self->m_label, str);
}

INLINE bool UIButton_isSelected(UIButton *self)
{
    assert(self && "The UIButton must be created");
    return self->m_state == UI_BUTTON_SELECTED;
}

INLINE bool UIButton_isPressed(UIButton *self)
{
    assert(self && "The UIButton must be created");
    return self->m_state == UI_BUTTON_PRESSED;
}
