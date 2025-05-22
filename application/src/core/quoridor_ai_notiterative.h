/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "core/quoridor_core.h"

/// @brief Calcule le coup joué par l'IA selon un algorithme de type min-max.
/// @param self Instance du jeu Quoridor.
/// @param depth Profondeur de la recherche dans l'arbre de jeu.
/// @param aiData Pointeur vers les données de l'IA.
/// @return Le tour choisi par l'IA.
QuoridorTurn QuoridorCore_computeOtherTurn(QuoridorCore *self, int depth, void *aiData);