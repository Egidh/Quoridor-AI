/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "core/quoridor_core.h"
#include "core/utils.h"

void *AIData_create(QuoridorCore *core)
{
    // Cette fonction n'est utile que si vous avez besoin de mémoriser des informations sur les coups précédents de l'IA.

    return NULL;
}

void AIData_destroy(void *self)
{
    if (!self) return;
}

void AIData_reset(void *self)
{
}

void QuoridorCore_getShortestPath(QuoridorCore *self, int playerID, QuoridorPos *path, int *size)
{
    *size = 0;

    // TODO
}

/// @brief Calcule une heuristique d'évaluation de l'état du jeu pour un joueur donné.
/// Cette fonction est utilisée dans l'algorithme Min-Max pour estimer la qualité d'une position.
/// Elle retourne une valeur représentant l'avantage du joueur playerID.
/// Une valeur positive indique un avantage pour ce joueur, une valeur négative indique un avantage pour l'adversaire.
/// @param self Instance du jeu Quoridor.
/// @param playerID Indice du joueur à évaluer (0 ou 1).
/// @return Une estimation numérique de l'avantage du joueur playerID.
static float QuoridorCore_computeScore(QuoridorCore *self, int playerID)
{
    int playerA = playerID;
    int playerB = playerID ^ 1;

    // TODO

    return 0.f + Float_randAB(-0.1f, +0.1f);
}

/// @brief Applique l'algorithme Min-Max (avec élagage alpha-bêta) pour déterminer le coup joué par l'IA.
/// Cette fonction explore récursivement une partie de l'arbre des coups possibles jusqu'à une profondeur maximale donnée.
/// @param self Instance du jeu Quoridor.
/// @param playerID Identifiant du joueur pour lequel on cherche le meilleur coup.
/// @param currDepth Profondeur actuelle dans l'arbre de recherche.
/// @param maxDepth Profondeur maximale à atteindre dans l'arbre.
/// @param alpha Meilleure valeur actuellement garantie pour le joueur maximisant.
/// @param beta Meilleure valeur actuellement garantie pour le joueur minimisant.
/// @param turn Pointeur vers une variable où sera enregistré le meilleur coup trouvé (à la racine).
/// @return L'évaluation numérique de la position courante, selon la fonction heuristique.
static float QuoridorCore_minMax(
    QuoridorCore *self, int playerID, int currDepth, int maxDepth,
    float alpha, float beta, QuoridorTurn *turn)
{
    if (self->state != QUORIDOR_STATE_IN_PROGRESS)
    {
        // TODO
    }
    else if (currDepth >= maxDepth)
    {
        return QuoridorCore_computeScore(self, playerID);
    }

    const int gridSize = self->gridSize;
    const int currI = self->positions[self->playerID].i;
    const int currJ = self->positions[self->playerID].j;
    QuoridorTurn childTurn = { 0 };

    const bool maximizing = (currDepth % 2) == 0;
    float value = maximizing ? -INFINITY : INFINITY;

    // TODO

    // Astuce :
    // vous devez effectuer toutes vos actions sur une copie du plateau courant.
    // Comme la structure QuoridorCore ne contient aucune allocation interne,
    // la copie s'éffectue simplement avec :
    // QuoridorCore gameCopy = *self;

    return value;
}

QuoridorTurn QuoridorCore_computeTurn(QuoridorCore *self, int depth, void *aiData)
{
    QuoridorTurn childTurn = { 0 };

    if (self->state != QUORIDOR_STATE_IN_PROGRESS) return childTurn;

    const float alpha = -INFINITY;
    const float beta = INFINITY;
    float childValue = QuoridorCore_minMax(self, self->playerID, 0, depth, alpha, beta, &childTurn);
    return childTurn;
}
