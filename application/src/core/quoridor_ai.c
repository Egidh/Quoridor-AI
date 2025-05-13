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

int Dijkstra_getNextNode(int *pred, int* dist, bool *explored, int size)
{
    int minDist = INT_MAX;
    int posMinDist = -1;

    for (int i = 0; i < size; i++)
    {
        if (dist[i] < minDist && !explored[i])
        {
            minDist = dist[i];
            posMinDist = i;
        }
    }
    return posMinDist;
}

void QuoridorCore_getShortestPath(QuoridorCore *self, int playerID, QuoridorPos *path, int *size)
{
    int tileNumber = self->gridSize * self->gridSize;

    int *pred = (int*)calloc(tileNumber, sizeof(int));
    int* dist = (int*)calloc(tileNumber, sizeof(int));
    bool* explored = (bool*)calloc(tileNumber, sizeof(bool));

    for (int i = 0; i < tileNumber; i++)
    {
        pred[i] = -1;
        dist[i] = INT_MAX;
    }
    int pos = self->positions[playerID].i * self->gridSize + self->positions[playerID].j;
    dist[pos] = 0;

    int u = Dijkstra_getNextNode(pred, dist, explored,  tileNumber);
    int i, j;
    while (u != -1)
    {
        i = u / self->gridSize;
        j = u % self->gridSize;
        explored[u] = true;
        if (!QuoridorCore_hasWallAbove(self, i, j))
        {
            if (dist[u] + 1 < dist[u - self->gridSize])
            {
                pred[u - self->gridSize] = u;
                dist[u - self->gridSize] = dist[u] + 1;
            }
        }
        if (!QuoridorCore_hasWallBelow(self, i, j))
        {
            if (dist[u] + 1 < dist[u + self->gridSize])
            {
                pred[u + self->gridSize] = u;
                dist[u + self->gridSize] = dist[u] + 1;
            }
        }
        if (!QuoridorCore_hasWallLeft(self, i, j))
        {
            if (dist[u] + 1 < dist[u - 1])
            {
                pred[u - 1] = u;
                dist[u - 1] = dist[u] + 1;
            }
        }
        if (!QuoridorCore_hasWallRight(self, i, j))
        {
            if (dist[u] + 1 < dist[u + 1])
            {
                pred[u + 1] = u;
                dist[u + 1] = dist[u] + 1;
            }
        }
        u = Dijkstra_getNextNode(pred, dist, explored, tileNumber);
    }

    int min = INT_MAX;
    int minIndex = -1;
    int offset = (playerID) ? 0 : self->gridSize - 1;
    for (int k = 0; k < self->gridSize; k++)
    {
        if (dist[offset + k * self->gridSize] < min)
        {
            min = dist[offset + k * self->gridSize];
            minIndex = offset + k * self->gridSize;
        }
    }

    *size = min + 1;
    for (int k = min - 1; k >= 0; k--)
    {
        path[k].i = pred[minIndex] / self->gridSize;
        path[k].j = pred[minIndex] % self->gridSize;

        minIndex = pred[minIndex];
    }
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

    QuoridorPos* my_path = (QuoridorPos*)calloc(MAX_PATH_LEN, sizeof(QuoridorPos));
    QuoridorPos* other_path = (QuoridorPos*)calloc(MAX_PATH_LEN, sizeof(QuoridorPos));
    int* my_distance;
    int* other_distance;

    QuoridorCore_getShortestPath(self, playerID, my_path, my_distance);
    QuoridorCore_getShortestPath(self, playerID ^ 1, other_path, other_distance);

    return my_distance - other_distance;
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
