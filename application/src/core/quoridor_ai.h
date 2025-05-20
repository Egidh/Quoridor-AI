/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "core/quoridor_core.h"

/// @brief Crée les données utilisées par l'IA.
/// @param core Instance du jeu Quoridor.
/// @return Pointeur vers les données de l'IA créées.
void *AIData_create(QuoridorCore *core);

/// @brief Détruit les données de l'IA.
/// @param self Pointeur vers les données de l'IA à détruire.
void AIData_destroy(void *self);

/// @brief Réinitialise les données de l'IA.
/// À appeler au début de chaque nouvelle partie.
/// @param self Pointeur vers les données de l'IA.
void AIData_reset(void *self);

/// @brief Calcule une heuristique d'évaluation de l'état du jeu pour un joueur donné.
/// Cette fonction est utilisée dans l'algorithme Min-Max pour estimer la qualité d'une position.
/// Elle retourne une valeur représentant l'avantage du joueur playerID.
/// Une valeur positive indique un avantage pour ce joueur, une valeur négative indique un avantage pour l'adversaire.
/// @param self Instance du jeu Quoridor.
/// @param playerID Indice du joueur à évaluer (0 ou 1).
/// @return Une estimation numérique de l'avantage du joueur playerID.
float QuoridorCore_computeScore(QuoridorCore* self, int playerID, QuoridorTurn turn);


/// @brief Calcule le coup joué par l'IA selon un algorithme de type min-max.
/// @param self Instance du jeu Quoridor.
/// @param depth Profondeur de la recherche dans l'arbre de jeu.
/// @param aiData Pointeur vers les données de l'IA.
/// @return Le tour choisi par l'IA.
QuoridorTurn QuoridorCore_computeTurn(QuoridorCore *self, int depth, void *aiData);

/// @brief Calcule le plus court chemin entre la position du joueur et sa zone d'arrivée.
/// @param self Instance du jeu Quoridor.
/// @param playerID Identifiant du joueur (0 ou 1).
/// @param path Tableau (préalloué, taille MAX_PATH_LEN) dans lequel sera écrit le plus court chemin.
///     La position finale (dans la zone de victoire) est stockée à l'indice 0,
///     et la position actuelle du joueur à l'indice (size - 1).
/// @param size Adresse d'un entier dans lequel sera écrite la taille du chemin.
///     La distance est alors égale à (size - 1).
void QuoridorCore_getShortestPath(QuoridorCore *self, int playerID, QuoridorPos *path, int *size);

/// @brief Fonction spécifique à l'évaluation sur Moodle.
/// Calcule le coup joué par l'IA avec une profondeur de 2.
/// @param self Instance du jeu Quoridor.
/// @param aiData Pointeur vers les données de l'IA.
/// @return Le tour choisi par l'IA.
INLINE QuoridorTurn QuoridorCore_computeMoodleTurn(QuoridorCore *self, void *aiData)
{
    const int depth = 2;
    return QuoridorCore_computeTurn(self, depth, aiData);
}