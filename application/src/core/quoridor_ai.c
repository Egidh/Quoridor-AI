/*
	Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
	Licensed under the MIT License.
	See LICENSE.md in the project root for license information.
*/

#include "core/quoridor_core.h"
#include "core/utils.h"

#define MAX_NUM_WALL 130

typedef struct TurnToSort {
	QuoridorTurn turn;
	int value;
}TurnToSort;

void* AIData_create(QuoridorCore* core)
{
	// Cette fonction n'est utile que si vous avez besoin de mémoriser des informations sur les coups précédents de l'IA.

	return NULL;
}

void AIData_destroy(void* self)
{
	if (!self) return;
}

void AIData_reset(void* self)
{
}

int Dijkstra_getNextNode(int* pred, unsigned int* dist, bool* explored, int size)
{
	unsigned int minDist = UINT_MAX;
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

void QuoridorCore_getShortestPath(QuoridorCore* self, int playerID, QuoridorPos* path, int* size)
{
	int tileNumber = self->gridSize * self->gridSize;

	int pred[MAX_GRID_SIZE * MAX_GRID_SIZE];
	memset(pred, -1, sizeof(pred));
	unsigned int dist[MAX_GRID_SIZE * MAX_GRID_SIZE];
	memset(dist, UINT_MAX, sizeof(dist));

	bool explored[MAX_GRID_SIZE * MAX_GRID_SIZE] = { 0 };

	int pos = self->positions[playerID].i * self->gridSize + self->positions[playerID].j;
	dist[pos] = 0;

	int jTarget = (playerID == 0) ? self->gridSize - 1 : 0;

	int u = Dijkstra_getNextNode(pred, dist, explored, tileNumber);
	int i, j;
	while (u != -1)
	{
		i = u / self->gridSize;
		j = u % self->gridSize;
		explored[u] = true;

		if (j == jTarget) break;

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

	int minIndex = i * self->gridSize + j;
	*size = dist[minIndex] + 1;
	for (int k = 0; k < *size; k++)
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
static float QuoridorCore_computeScore(QuoridorCore* self, int playerID)
{
	int playerA = playerID;
	int playerB = playerID ^ 1;

	QuoridorPos my_path[MAX_PATH_LEN];
	QuoridorPos other_path[MAX_PATH_LEN];

	int my_distance;
	int other_distance;

	int currI = self->positions[playerID].i;
	int currJ = self->positions[playerID].j;

	int otherI = self->positions[playerID ^ 1].i;
	int otherJ = self->positions[playerID ^ 1].j;


	QuoridorCore_getShortestPath(self, playerID, my_path, &my_distance);
	QuoridorCore_getShortestPath(self, playerID ^ 1, other_path, &other_distance);

	my_distance--;
	other_distance--;

	if (my_distance == 0)
		return 10000;
	if (other_distance == 0)
		return -10000;

	float score = 0;
	score += (other_distance - my_distance) * 2.5;

	return score + Float_rand01();
}

static float QuoridorCore_computeWall(QuoridorPos* myPath, int myLen, QuoridorPos* otherPath, int otherLen, QuoridorTurn turn)
{
	float score = 0;
	score += (otherLen - myLen)*2.5;
	for (int i = 0; i < otherLen; i++)
	{
		if (turn.i == otherPath[i].i)
			score++;
		if (turn.j == otherPath[i].j)
			score++;
	}
	return score + Float_rand01();
}

int QuoridorCore_compareWall(const void* first, const void* second)
{
	TurnToSort a = *(TurnToSort*)first;
	TurnToSort b = *(TurnToSort*)second;
	if (a.turn.action == QUORIDOR_ACTION_UNDEFINED) return 1;
	if (b.turn.action == QUORIDOR_ACTION_UNDEFINED) return -1;
	if (a.value < b.value) return 1;
	if (a.value > b.value) return -1;
	else return 0;
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
	QuoridorCore* self, int playerID, int currDepth, int maxDepth,
	float alpha, float beta, QuoridorTurn* turn)
{
	switch (self->state)
	{
	case QUORIDOR_STATE_P0_WON:
		return (playerID == 1) ? -10000 + currDepth * 2 : 10000 - currDepth * 2;
	case QUORIDOR_STATE_P1_WON:
		return (playerID == 1) ? 10000 - currDepth * 2 : -10000 + currDepth * 2;
	default:
		break;
	}

	if (currDepth >= maxDepth) return QuoridorCore_computeScore(self, playerID);

	const int gridSize = self->gridSize;

	const int currI = self->positions[self->playerID].i;
	const int currJ = self->positions[self->playerID].j;
	const int otherI = self->positions[self->playerID ^ 1].i;
	const int otherJ = self->positions[self->playerID ^ 1].j;

	QuoridorTurn childTurn = { 0 };

	// TODO

	// Astuce :
	// vous devez effectuer toutes vos actions sur une copie du plateau courant.
	// Comme la structure QuoridorCore ne contient aucune allocation interne,
	// la copie s'éffectue simplement avec :
	// QuoridorCore gameCopy = *self;

	bool maximizing = (!(currDepth & 1)) ? true : false;
	float value = (maximizing) ? -INFINITY : INFINITY;
	float currValue;

	QuoridorCore gameCopy = *self;
	QuoridorTurn currTurn;

	QuoridorPos myPath[MAX_PATH_LEN];
	QuoridorPos otherPath[MAX_PATH_LEN];
	int mySize, otherSize;
	QuoridorCore_getShortestPath(self, playerID, myPath, &mySize);
	QuoridorCore_getShortestPath(self, playerID ^ 1, otherPath, &otherSize);

	TurnToSort list[MAX_NUM_WALL] = { 0 };
	size_t pos = 0;

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			// Déplacement
			if (self->isValid[i][j])
			{
				currTurn.action = QUORIDOR_MOVE_TO;
				currTurn.i = i;
				currTurn.j = j;

				QuoridorCore_playTurn(&gameCopy, currTurn);
				currValue = QuoridorCore_minMax(&gameCopy, playerID, currDepth + 1, maxDepth, alpha, beta, &childTurn);

				if ((maximizing && currValue > value) || (!maximizing) && currValue < value)
				{
					value = currValue;
					*turn = currTurn;
				}

				if (maximizing && value >= beta || !maximizing && value <= alpha)
					return value;

				alpha = (alpha < value && maximizing) ? value : alpha;
				beta = (beta > value && !maximizing) ? value : beta;

				gameCopy = *self;
			}

			if (QuoridorCore_canPlayWall(&gameCopy, WALL_TYPE_HORIZONTAL, i, j))
			{
				list[pos].turn.action = QUORIDOR_PLAY_HORIZONTAL_WALL;
				list[pos].turn.i = i;
				list[pos].turn.j = j;
				list[pos].value = QuoridorCore_computeWall(myPath, mySize, otherPath, otherSize, list[pos++].turn);
			}
			if (QuoridorCore_canPlayWall(&gameCopy, WALL_TYPE_VERTICAL, i, j))
			{
				list[pos].turn.action = QUORIDOR_PLAY_VERTICAL_WALL;
				list[pos].turn.i = i;
				list[pos].turn.j = j;
				list[pos].value = QuoridorCore_computeWall(myPath, mySize, otherPath, otherSize, list[pos++].turn);
			}
		}
	}

	if(pos > 0)
	{
		qsort(list, pos, sizeof(TurnToSort), QuoridorCore_compareWall);

		for (int i = 0; i < 5; i++)
		{
			QuoridorCore_playTurn(&gameCopy, list[i].turn);
			currValue = QuoridorCore_minMax(&gameCopy, playerID, currDepth + 1, maxDepth, alpha, beta, &childTurn);

			if ((maximizing && currValue > value) || (!maximizing) && currValue < value)
			{
				value = currValue;
				*turn = list[i].turn;
			}

			if (maximizing && value >= beta || !maximizing && value <= alpha)
				return value;

			alpha = (alpha < value && maximizing) ? value : alpha;
			beta = (beta > value && !maximizing) ? value : beta;

			gameCopy = *self;
		}
	}


	return value;
}

QuoridorTurn QuoridorCore_computeTurn(QuoridorCore* self, int depth, void* aiData)
{
	QuoridorTurn childTurn = { 0 };

	if (self->state != QUORIDOR_STATE_IN_PROGRESS) return childTurn;

	const float alpha = -INFINITY;
	const float beta = INFINITY;
	float childValue = QuoridorCore_minMax(self, self->playerID, 0, depth, alpha, beta, &childTurn);
	return childTurn;
}
