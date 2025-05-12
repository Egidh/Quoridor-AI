/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "engine/math.h"
#include "game/game_common.h"

typedef struct PlayerInput
{
    bool validatePressed;
} PlayerInput;

/// @brief Structure représentant le gestionnaire des entrées utilisateur.
typedef struct Input
{
    /// @brief Booléen indiquant si le bouton "quitter" vient d'être pressé.
    bool quitPressed;

    bool validatePressed;

    PlayerInput players[MAX_PLAYER_COUNT];

    Vec2 mousePos;
} Input;

/// @brief Crée un nouveau gestionnaire des entrées utilisateur.
/// @return Le gestionnaire créé.
Input *Input_create();

/// @brief Détruit le gestionnaire des entrées utilisateur.
/// @param self le gestionnaire.
void Input_destroy(Input *self);

/// @brief Met à jour le gestionnaire des entrées utilisateur.
/// Cette fonction effectue la boucle des événements SDL.
/// @param self le gestionnaire.
void Input_update(Input *self);

void Input_updateControllerButtonDown(Input *self, PlayerInput *playerInput, int button);
void Input_updateControllerButtonUp(Input *self, PlayerInput *playerInput, int button);
void Input_updateControllerAxisMotion(Input *self, PlayerInput *playerInput, int axis, Sint16 value);
void Input_updateKeyDown(Input *self, PlayerInput *playerInput, bool repeat, int scancode);
void Input_updateKeyUp(Input *self, PlayerInput *playerInput, bool repeat, int scancode);
void Input_updateMouseButtonDown(Input *self, int button);
void Input_updateMouseButtonUp(Input *self, int button);
