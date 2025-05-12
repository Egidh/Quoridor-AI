/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "core/quoridor_core.h"
#include "core/quoridor_ai.h"

void QuoridorCore_updateValidMoves(QuoridorCore *self);
bool QuoridorCore_isFeasible(QuoridorCore *self);

QuoridorCore *QuoridorCore_create()
{
    QuoridorCore *self = (QuoridorCore *)calloc(1, sizeof(QuoridorCore));
    AssertNew(self);

    QuoridorCore_reset(self, 9, 10, 0);

    return self;
}

void QuoridorCore_destroy(QuoridorCore *self)
{
    if (!self) return;
    free(self);
}

void QuoridorCore_reset(QuoridorCore *self, int gridSize, int wallCount, int firstPlayer)
{
    memset(self, 0, sizeof(QuoridorCore));

    self->gridSize = gridSize;
    self->playerID = firstPlayer;

    self->positions[0].i = gridSize / 2;
    self->positions[0].j = 0;
    self->positions[1].i = gridSize / 2;
    self->positions[1].j = gridSize - 1;

    self->wallCounts[0] = wallCount;
    self->wallCounts[1] = wallCount;

    QuoridorCore_updateValidMoves(self);
}

void QuoridorCore_randomStart(QuoridorCore *self)
{
    self->wallCounts[0] += 2;
    self->wallCounts[1] += 2;

    const int gridSize = self->gridSize;

    for (int cpt = 0; cpt < 4; cpt++)
    {
        while (true)
        {
            int i = rand() % (gridSize - 1);
            int j = rand() % (gridSize - 1);
            int type = rand() & 1 ? WALL_TYPE_HORIZONTAL : WALL_TYPE_VERTICAL;
            if (QuoridorCore_canPlayWall(self, type, i, j))
            {
                QuoridorCore_playWall(self, type, i, j);
                break;
            }
        }
    }
}

bool QuoridorCore_canPlayWall(QuoridorCore *self, WallType type, int i, int j)
{
    if (i < 0 || i >= self->gridSize - 1) return false;
    if (j < 0 || j >= self->gridSize - 1) return false;

    // TODO

    return false;
}

void QuoridorCore_updateValidMoves(QuoridorCore *self)
{
    const int gridSize = self->gridSize;
    const int currI = self->positions[self->playerID].i;
    const int currJ = self->positions[self->playerID].j;
    const int otherI = self->positions[self->playerID ^ 1].i;
    const int otherJ = self->positions[self->playerID ^ 1].j;

    // TODO

    // Par exemple :
    self->isValid[otherI][otherJ] = false;
}

static bool QuoridorCore_isFeasibleRec0(QuoridorCore *self, bool explored[MAX_GRID_SIZE][MAX_GRID_SIZE], int i, int j)
{
    const int gridSize = self->gridSize;
    if (j >= gridSize - 1) return true;

    // TODO
    // Fonction récursive vérifiant qu'il existe un chemin permettant au joueur 0 de terminer la partie.
    // Cette fonction réalise un parcours en profondeur.
    // Ordonnez les cases voisines à explorer pour améliorer les performances.

    return false;
}

static bool QuoridorCore_isFeasibleRec1(QuoridorCore *self, bool explored[MAX_GRID_SIZE][MAX_GRID_SIZE], int i, int j)
{
    const int gridSize = self->gridSize;
    if (j <= 0) return true;

    // TODO
    // Fonction récursive vérifiant qu'il existe un chemin permettant au joueur 1 de terminer la partie.
    // Cette fonction réalise un parcours en profondeur.
    // Ordonnez les cases voisines à explorer pour améliorer les performances.

    return false;
}

bool QuoridorCore_isFeasible(QuoridorCore *self)
{
    bool explored[MAX_GRID_SIZE][MAX_GRID_SIZE] = { 0 };

    // TODO
    // Utilisez QuoridorCore_isFeasibleRec0().
    // Utilisez QuoridorCore_isFeasibleRec1().

    return false;
}

bool QuoridorCore_canMoveTo(QuoridorCore *self, int nextI, int nextJ)
{
    const int gridSize = self->gridSize;
    if (nextI < 0 || nextI >= gridSize) return false;
    if (nextJ < 0 || nextJ >= gridSize) return false;
    if (self->state != QUORIDOR_STATE_IN_PROGRESS) return false;
    return self->isValid[nextI][nextJ];
}

void QuoridorCore_playWall(QuoridorCore *self, WallType type, int i, int j)
{
    assert(0 <= i && i < self->gridSize - 1);
    assert(0 <= j && j < self->gridSize - 1);
    assert(self->wallCounts[self->playerID] > 0);
    
    // TODO
    // Placez le mur.

    self->playerID ^= 1;
    QuoridorCore_updateValidMoves(self);
}

void QuoridorCore_moveTo(QuoridorCore *self, int i, int j)
{
    assert(0 <= i && i < self->gridSize);
    assert(0 <= j && j < self->gridSize);


    // TODO
    // Déplacez le pion.
    // Changez l'état de la partie si le joueur vient de gagner.

    self->playerID ^= 1;
    QuoridorCore_updateValidMoves(self);
}

void QuoridorCore_playTurn(QuoridorCore *self, QuoridorTurn turn)
{
    if (turn.action == QUORIDOR_MOVE_TO)
    {
        assert(QuoridorCore_canMoveTo(self, turn.i, turn.j));
        QuoridorCore_moveTo(self, turn.i, turn.j);
    }

    // TODO
    // Complétez les autres actions possibles.
}

void QuoridorCore_print(QuoridorCore *self)
{
    const int gridSize = self->gridSize;
    for (int i = 0; i < gridSize; i++)
    {
        printf("+-");
    }
    printf("+\n");

    for (int i = 0; i < gridSize; i++)
    {
        printf("|");
        for (int j = 0; j < gridSize; j++)
        {
            if (self->positions[0].i == i && self->positions[0].j == j)
                printf("0");
            else if (self->positions[1].i == i && self->positions[1].j == j)
                printf("1");
            else
                printf(".");

            if (self->vWalls[i][j] == WALL_STATE_START)
                printf("A");
            else if (self->vWalls[i][j] == WALL_STATE_END)
                printf("B");
            else
                printf("|");
        }
        printf("\n+");

        for (int j = 0; j < gridSize; j++)
        {
            if (self->hWalls[i][j] == WALL_STATE_START)
                printf("A=");
            else if (self->hWalls[i][j] == WALL_STATE_END)
                printf("B|");
            else if (self->vWalls[i][j] == WALL_STATE_START)
                printf("-#");
            else
                printf("-+");
        }
        printf("\n");
    }
    printf("\n");
}
