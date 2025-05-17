/*
	Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
	Licensed under the MIT License.
	See LICENSE.md in the project root for license information.
*/

#include "core/quoridor_core.h"
#include "core/utils.h"

#ifndef DEBUG
#define DEBUG 0
#include <limits.h>
#endif

#define DIJKSTRA 0
#define BFS 1

#define MAX_NUM_WALL 130

typedef struct TurnToSort {
	QuoridorTurn turn;
	float value;
}TurnToSort;

typedef struct QuoridorPath {
	QuoridorPos tiles[MAX_PATH_LEN];
	int size;
} QuoridorPath;

/// @brief Indique si a est compris entre lowlimit et highlimit
/// @brief fonction rajoutée par Samuel
/// @param a La valeur à vérifier
/// @param lowlimit La valeur max autorisée pour a
/// @param highlimit La valeur min autorisée pour a
/// @return True si lowlimit <= a <= highlimit, false sinon
static bool isBetween(int a, int lowlimit, int highlimit)
{
	return (a >= lowlimit && a <= highlimit);
}

void* AIData_create(QuoridorCore* core)
{
	return NULL;
}

void AIData_destroy(void* self)
{
	if (!self) return;
}

void AIData_reset(void* self)
{
}

#if DIJKSTRA
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

		if (j == jTarget) break;
		u = Dijkstra_getNextNode(pred, dist, explored, tileNumber);
	}

	int minIndex = i * self->gridSize + j;
	*size = dist[minIndex] + 1;

	path[0].i = i;
	path[0].j = j;

	for (int k = 1; k < *size; k++)
	{
		path[k].i = pred[minIndex] / self->gridSize;
		path[k].j = pred[minIndex] % self->gridSize;

		minIndex = pred[minIndex];
	}
}
#endif

#if BFS
void QuoridorCore_getShortestPath(QuoridorCore* self, int playerID, QuoridorPos* path, int* size)
{
	int gridSize = self->gridSize;

	int targetJ = (playerID == 0) ? gridSize - 1 : 0;

	bool explored[MAX_GRID_SIZE][MAX_GRID_SIZE] = { 0 };

	QuoridorPos pred[MAX_GRID_SIZE][MAX_GRID_SIZE];
	memset(pred, -1, sizeof(pred));

	unsigned int dist[MAX_GRID_SIZE][MAX_GRID_SIZE];
	memset(dist, UINT_MAX, sizeof(dist));

	QuoridorPos file[MAX_GRID_SIZE * MAX_GRID_SIZE];
	memset(file, -1, sizeof(file));
	int readCursor = 0;
	int writeCursor = 0;
	file[writeCursor].i = self->positions[playerID].i;
	file[writeCursor++].j = self->positions[playerID].j;

	int i = file[readCursor].i, j = file[readCursor].j;
	dist[i][j] = 0;

	while (file[readCursor].i != -1 && readCursor < (gridSize * gridSize))
	{
		i = file[readCursor].i;
		j = file[readCursor++].j;

		if (j == targetJ) break;

		if (!QuoridorCore_hasWallAbove(self, i, j) && !explored[i - 1][j])
		{
			file[writeCursor].i = i - 1;
			file[writeCursor++].j = j;

			pred[i - 1][j].i = i;
			pred[i - 1][j].j = j;

			explored[i - 1][j] = true;
			dist[i - 1][j] = dist[i][j] + 1;
		}
		if (!QuoridorCore_hasWallBelow(self, i, j) && !explored[i + 1][j])
		{
			file[writeCursor].i = i + 1;
			file[writeCursor++].j = j;

			pred[i + 1][j].i = i;
			pred[i + 1][j].j = j;

			explored[i + 1][j] = true;
			dist[i + 1][j] = dist[i][j] + 1;
		}
		if (!QuoridorCore_hasWallLeft(self, i, j) && !explored[i][j - 1])
		{
			file[writeCursor].i = i;
			file[writeCursor++].j = j - 1;

			pred[i][j - 1].i = i;
			pred[i][j - 1].j = j;

			explored[i][j - 1] = true;
			dist[i][j - 1] = dist[i][j] + 1;
		}
		if (!QuoridorCore_hasWallRight(self, i, j) && !explored[i][j + 1])
		{
			file[writeCursor].i = i;
			file[writeCursor++].j = j + 1;

			pred[i][j + 1].i = i;
			pred[i][j + 1].j = j;

			explored[i][j + 1] = true;
			dist[i][j + 1] = dist[i][j] + 1;
		}
	}

	*size = (int)dist[i][j] + 1;
	QuoridorPos actual = { i, j };

	for (int k = 0; k < *size; k++)
	{
		path[k].i = actual.i;
		path[k].j = actual.j;

		actual = pred[actual.i][actual.j];
	}
}
#endif

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

	QuoridorPath my_path;
	QuoridorPath other_path;

	QuoridorCore_getShortestPath(self, playerA, my_path.tiles, &(my_path.size));
	QuoridorCore_getShortestPath(self, playerB, other_path.tiles, &(other_path.size));

	int mySpawn = (playerID == 0) ? 0 : self->gridSize - 1;
	int otherSpawn = (playerID ^ 1 == 0) ? 0 : self->gridSize - 1;

	float score = 0;
	// Différence de distance à parcourir
	score += (other_path.size - my_path.size) * 3;
	//Nombre de murs restants
	score += self->wallCounts[playerID] * 2;

	// Temps à passer de retour au départ
	for (int i = 0; i < my_path.size; i++) {
		if (my_path.tiles[i].j == mySpawn)
			score -= 2.5;
	}
	for (int i = 0; i < other_path.size; i++) {
		if (other_path.tiles[i].j == otherSpawn)
			score += 2.5;
	}

	// Distance du centre
	int center = self->gridSize / 2;
	score += (abs(self->positions[playerB].i - center) + abs(self->positions[playerB].j - center)) * 2;

	// Condition victoire / défaite
	if (my_path.size - 1 == 1)
		score += (self->playerID == playerID) ? 9000 : 8000;
	else if (other_path.size - 1 == 1)
		score -= (self->playerID == playerB) ? 9000 : 7000;

	return score + Float_rand01();
}

static float QuoridorCore_computeWall(QuoridorCore self, int playerID, QuoridorPath my_path, QuoridorPath other_path, QuoridorTurn turn)
{
	// Un coup de la victoire / défaite
	if (turn.action == QUORIDOR_PLAY_VERTICAL_WALL && other_path.size - 1 == 1 && (turn.i == other_path.tiles[1].i || turn.i == other_path.tiles[1].i - 1) && turn.j == other_path.tiles[playerID].j)
		return (playerID == self.playerID) ? 10000 : -10000;

	float score = 0;
	score += (other_path.size - my_path.size) * 3;

	int mySpawn = (playerID == 0) ? 0 : self.gridSize - 1;
	int otherSpawn = (playerID ^ 1 == 0) ? 0 : self.gridSize - 1;

	for (int i = 0; i < other_path.size; i++) {
		int distance = abs(turn.i - other_path.tiles[i].i) + abs(turn.j - other_path.tiles[i].j);
		score += (4 - distance) * 2;

		if (other_path.tiles[i].j == otherSpawn)
			score += 2.5;
	}

	for (int i = turn.i - 1; i >= 0; i--) {
		if (self.vWalls[i][turn.j] != WALL_STATE_NONE) score += 1.5;
		else break;
	}
	for (int i = turn.i + 2; i < self.gridSize; i++) {
		if (self.vWalls[i][turn.j] != WALL_STATE_NONE) score += 1.5;
		else break;
	}

	if (abs(turn.i - self.gridSize / 2) <= 1) score += 1;
	if (abs(turn.j - self.gridSize / 2) <= 1) score += 1;

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

#if DEBUG
static int movesAtDepth[10] = { 0 };
static int nodeVisited = 0;
static float totalValues = 0;
#endif

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
	float alpha, float beta, QuoridorTurn* turn, void* AIdata)
{
#if DEBUG
	nodeVisited++;
#endif
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

	QuoridorPath my_path;
	QuoridorPath other_path;

	QuoridorCore_getShortestPath(self, playerID, my_path.tiles, &(my_path.size));
	QuoridorCore_getShortestPath(self, playerID ^ 1, other_path.tiles, &(other_path.size));

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
				currValue = QuoridorCore_minMax(&gameCopy, playerID, currDepth + 1, maxDepth, alpha, beta, &childTurn, AIdata);
				gameCopy = *self;
#if DEBUG
				totalValues += currValue;
				movesAtDepth[currDepth]++;
#endif


				if ((maximizing && currValue > value) || ((!maximizing) && currValue < value))
				{
					value = currValue;
					*turn = currTurn;
				}

				if ((maximizing && value >= beta) || (!maximizing && value <= alpha))
					return value;

				alpha = (alpha < value && maximizing) ? value : alpha;
				beta = (beta > value && !maximizing) ? value : beta;

			}

			if (QuoridorCore_canPlayWall(&gameCopy, WALL_TYPE_VERTICAL, i, j))
			{
				list[pos].turn.action = QUORIDOR_PLAY_VERTICAL_WALL;
				list[pos].turn.i = i;
				list[pos].turn.j = j;
				list[pos].value = QuoridorCore_computeWall(gameCopy, playerID, my_path, other_path, list[pos].turn);
				pos++;
			}

			if (QuoridorCore_canPlayWall(&gameCopy, WALL_TYPE_HORIZONTAL, i, j))
			{
				list[pos].turn.action = QUORIDOR_PLAY_HORIZONTAL_WALL;
				list[pos].turn.i = i;
				list[pos].turn.j = j;
				list[pos].value = QuoridorCore_computeWall(gameCopy, playerID, my_path, other_path, list[pos].turn);
				pos++;
			}

		}
	}

	if (pos > 0)
	{
		qsort(list, pos, sizeof(TurnToSort), QuoridorCore_compareWall);

		int limit = (pos < 5) ? pos : 5;
		for (int i = 0; i < limit; i++)
		{
			QuoridorCore_playTurn(&gameCopy, list[i].turn);
			currValue = QuoridorCore_minMax(&gameCopy, playerID, currDepth + 1, maxDepth, alpha, beta, &childTurn, AIdata);
			gameCopy = *self;
#if DEBUG
			movesAtDepth[currDepth]++;
			totalValues += currValue;
#endif

			if ((maximizing && currValue > value) || ((!maximizing) && currValue < value))
			{
				value = currValue;
				*turn = list[i].turn;
			}

			if ((maximizing && value >= beta) || (!maximizing && value <= alpha))
				return value;

			alpha = (alpha < value && maximizing) ? value : alpha;
			beta = (beta > value && !maximizing) ? value : beta;

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

#if DEBUG == 0
	float childValue = QuoridorCore_minMax(self, self->playerID, 0, depth, alpha, beta, &childTurn, aiData);
#endif

#if DEBUG
	float childValue = QuoridorCore_minMax(self, self->playerID, 0, 5, alpha, beta, &childTurn, aiData);
	for (int i = 0; i < 10; i++)
		printf("%d mouvements etudies a la profondeur %d\n", movesAtDepth[i], i);

	printf("Total : %d noeuds\n", nodeVisited);
	printf("Valeur du coup : %f, valeur moyenne des coups evalues : %f\n", childValue, totalValues / nodeVisited);

	memset(movesAtDepth, 0, sizeof(movesAtDepth));
	nodeVisited = 0;
	totalValues = 0;
#endif

	return childTurn;
}
