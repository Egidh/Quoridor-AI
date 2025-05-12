/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Structure représentant une animation.
typedef struct SpriteAnim
{
    int m_frameCount;
    int m_currFrameIdx;
    int m_prevFrameIdx;
    float m_cycleTime;
    int m_cycleCount;
    int m_cycleIdx;
    float m_accu;
    bool m_stopped;
} SpriteAnim;

/// @brief Crée une animation.
/// @param rectCount le nombre d'images de l'animation.
/// @param cycleTime la durée d'un cycle de l'animation.
/// @param cycleCount le nombre de cycle de l'animation,
///     ou -1 si l'animation tourne en boucle.
/// @return L'animation créée.
SpriteAnim *SpriteAnim_create(int rectCount, float cycleTime, int cycleCount);

/// @brief Détruit une animation.
/// @param self l'animation.
void SpriteAnim_destroy(SpriteAnim *self);

/// @brief Met à jour une animation.
/// @param self l'animation.
/// @param dt l'écart de temps écoulé depuis le dernier rendu.
void SpriteAnim_update(SpriteAnim *self, float dt);

/// @brief Renvoie l'indice de l'image à rendre.
/// @param self l'animation.
/// @return L'indice de l'image à rendre.
int SpriteAnim_getFrameIndex(SpriteAnim *self);

/// @brief Indique si l'image à afficher a changée lors de la dernière mise à jour.
/// @param self l'animation.
/// @return La valeur true si l'image à changée, false sinon.
bool SpriteAnim_frameChanged(SpriteAnim *self);

/// @brief Change une animation.
/// @param self l'animation.
/// @param rectCount le nombre d'images de l'animation.
/// @param cycleTime la durée d'un cycle de l'animation.
/// @param cycleCount le nombre de cycle de l'animation,
///     ou -1 si l'animation tourne en boucle.
void SpriteAnim_set(SpriteAnim *self, int rectCount, float cycleTime, int cycleCount);

/// @brief Réinitialise une animation.
/// @param self l'animation.
void SpriteAnim_restart(SpriteAnim *self);

void SpriteAnim_stop(SpriteAnim *self);

/// @brief Indique si une animation est terminée.
/// Si le nombre de cycles d'une animation vaut -1, elle ne se termine jamais.
/// @param self l'animation.
/// @return La valeur true si l'animation est terminée, false sinon.
bool SpriteAnim_isFinished(SpriteAnim *self);
