/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/math.h"

/// @brief Structure représentant une caméra.
typedef struct Camera
{
    /// @brief Rectangle représentant la vue de la caméra.
    /// Ces coordonnées sont dans le référentiel monde.
    AABB m_worldView;

    SDL_FRect m_viewport;
} Camera;

/// @brief Crée une nouvelle caméra.
/// @return La caméra créée.
Camera *Camera_create();

void Camera_updateViewport(Camera *self, SDL_Renderer *renderer);

/// @brief Détruit une caméra.
/// @param self la caméra.
void Camera_destroy(Camera *self);

/// @brief Renvoie le rectangle vu par la caméra.
/// Les coordonnées sont exprimées dans le référentiel monde.
/// @param self la caméra.
/// @return Le rectangle vu par la caméra.
INLINE AABB Camera_getWorldView(Camera *self)
{
    assert(self && "The Camera must be created");
    return self->m_worldView;
}

/// @brief Définit le rectangle vu par la caméra.
/// Les coordonnées sont exprimées dans le référentiel monde.
/// @param self la caméra.
/// @param worldView le rectangle.
INLINE void Camera_setWorldView(Camera *self, AABB worldView)
{
    assert(self && "The Camera must be created");
    self->m_worldView = worldView;
}

/// @brief Déplace le rectangle vu par la caméra.
/// Les coordonnées sont exprimées dans le référentiel monde.
/// @param self la caméra.
/// @param displacement le vecteur de translation (dans le référentiel monde).
void Camera_translateWorldView(Camera *self, Vec2 displacement);

/// @brief Renvoie la largeur en pixels de la caméra.
/// @param self la caméra.
/// @return La largeur en pixels de la caméra.
float Camera_getWidth(Camera *self);

/// @brief Renvoie la hauteur en pixels de la caméra.
/// @param self la caméra.
/// @return La hauteur en pixels de la caméra.
float Camera_getHeight(Camera *self);

/// @brief Renvoie le facteur d'échelle par lequel il faut multiplier des
/// distances exprimées dans le reférentiel monde pour obtenir des distances
/// exprimées dans le référentiel vue (en pixels).
/// @param self la caméra.
/// @return Le facteur d'échelle monde vers vue (en pixels).
float Camera_getWorldToViewScale(Camera *self);

/// @brief Transforme des coordonnées exprimées dans le référentiel monde vers
/// le référentiel vue (en pixels).
/// @param[in] self la caméra.
/// @param[in] position la position d'un point dans le référentiel monde.
/// @param[out] x l'abscisse du point dans la vue (en pixels).
/// @param[out] y l'ordonnée du point dans la vue (en pixels).
void Camera_worldToView(Camera *self, Vec2 position, float *x, float *y);

float Camera_worldToViewX(Camera *self, float positionX);
float Camera_worldToViewY(Camera *self, float positionY);

/// @brief Transforme des coordonnées exprimée dans le référentiel de la vue
/// (en pixels) vers le référentiel monde.
/// @param[in] self camera la caméra.
/// @param[in] x l'abscisse d'un point dans le référentiel vue (en pixels).
/// @param[in] y l'ordonnée d'un point dans le référentiel vue (en pixels).
/// @param[out] position la position du point dans le référentiel monde.
void Camera_viewToWorld(Camera *self, float x, float y, Vec2 *position);
