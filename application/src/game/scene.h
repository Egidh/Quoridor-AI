/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/common.h"
#include "engine/camera.h"
#include "core/quoridor_core.h"
#include "core/quoridor_ai.h"
#include "game/game_common.h"
#include "game/input.h"
#include "game/ui_quoridor.h"

/// @brief Structure représentant la scène du menu principal du jeu.
typedef struct Scene
{
    GameConfig *m_gameConfig;
    AssetManager *m_assets;
    Camera *m_camera;
    Input *m_input;
    UIQuoridor *m_quoridorUI;
    QuoridorCore *m_core;

    int m_state;
    float m_accu;
    float m_fadingTime;
} Scene;

/// @brief Crée la scène représentant le menu principal du jeu.
/// @param gameConfig la configuration globale du jeu.
/// @return La scène créée.
Scene *Scene_create(GameConfig *gameConfig);

/// @brief Détruit la scène représentant le menu principal du jeu.
/// @param self la scène.
void Scene_destroy(Scene *self);

/// @brief Boucle principale de la scène.
/// @param self la scène.
/// @param drawGizmos booléen indiquant s'il faut dessiner les gizmos.
void Scene_mainLoop(Scene *self, bool drawGizmos);

/// @brief Met à jour la scène.
/// Cette fonction est appelée à chaque tour de la boucle de rendu.
/// @param self la scène.
void Scene_update(Scene *self);

/// @brief Active l'animation de fin de scène.
/// La boucle principale s'arrête une fois l'animation terminée.
/// @param self la scène.
void Scene_quit(Scene *self);

/// @brief Dessine la scène dans le moteur de rendu.
/// @param self la scène.
void Scene_render(Scene *self);

/// @brief Dessine les gizmos de la scène dans le moteur de rendu.
/// @param self la scène.
void Scene_drawGizmos(Scene *self);

/// @brief Renvoie le gestionnaire des assets de la scène.
/// @param self la scène.
/// @return Le gestionnaire des assets de la scène.
INLINE AssetManager *Scene_getAssetManager(Scene *self)
{
    assert(self && "The Scene must be created");
    return self->m_assets;
}

/// @brief Renvoie le gestionnaire des entrées utilisateur de la scène.
/// @param self la scène.
/// @return Le gestionnaire des entrées utilisateur de la scène.
INLINE Input *Scene_getInput(Scene *self)
{
    assert(self && "The Scene must be created");
    return self->m_input;
}

/// @brief Renvoie la caméra de la scène.
/// @param self la scène.
/// @return La caméra de la scène.
INLINE Camera *Scene_getCamera(Scene *self)
{
    assert(self && "The Scene must be created");
    return self->m_camera;
}

INLINE QuoridorCore *Scene_getQuoridorCore(Scene *self)
{
    assert(self && "The Scene must be created");
    return self->m_core;
}