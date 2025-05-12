/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Structure représentant d'un texte affichable.
typedef struct Text
{
    char *m_str;
    TTF_Font *m_font;
    SDL_Color m_color;
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
} Text;

/// @brief Crée un texte affichable.
/// @param renderer le moteur de rendu.
/// @param font la police utilisée pour rendre le texte.
/// @param str la chaîne associée au texte.
/// @param color la couleur de rendu.
/// @return Le texte affichable créé.
Text *Text_create(SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color);

/// @brief Détruit un texte affichable.
/// @param self le texte.
void Text_destroy(Text *self);

/// @brief Change le contenu du texte.
/// @param self le texte.
/// @param str la nouvelle chaîne à rendre.
void Text_setString(Text *self, const char *str);

/// @brief Change le couleur de rendu d'un texte affichable.
/// @param self le texte.
/// @param color la couleur.
void Text_setColor(Text *self, SDL_Color color);

/// @brief Renvoie la chaîne associée à un texte affichable.
/// @param self le texte.
/// @return La chaîne (contenu) du texte.
INLINE const char *Text_getString(Text *self)
{
    return (const char*)(self->m_str);
}

/// @brief Renvoie la texture d'un texte affichable.
/// @param self le texte.
/// @return La texture du texte.
INLINE SDL_Texture *Text_getTexture(Text *self)
{
    return self->m_texture;
}

INLINE int Text_getHeight(Text *self)
{
    assert(self && "The Text must be created");
    if (self->m_texture) return self->m_texture->h;
    else return 0;
}

INLINE int Text_getWidth(Text *self)
{
    assert(self && "The Text must be created");
    if (self->m_texture) return self->m_texture->w;
    else return 0;
}

void Text_getSize(Text *self, float *w, float *h);
