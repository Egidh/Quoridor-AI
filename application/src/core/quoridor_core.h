/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Taille maximale (largeur et hauteur) du plateau du jeu Quoridor.
#define MAX_GRID_SIZE 9

/// @brief Longueur maximale d'un chemin sur le plateau.
#define MAX_PATH_LEN 82

typedef enum WallState
{
    /// @brief Indique l'absence d'un mur.
    WALL_STATE_NONE,

    /// @brief Indique le début d'un mur
    /// (partie gauche pour un mur horizontal, partie haute pour un mur vertical).
    WALL_STATE_START,

    /// @brief Indique la fin d'un mur
    /// (partie droite pour un mur horizontal, partie basse pour un mur vertical).
    WALL_STATE_END
} WallState;

typedef enum WallType
{
    /// @brief Mur horizontal.
    WALL_TYPE_HORIZONTAL,

    /// @brief Mur vertical.
    WALL_TYPE_VERTICAL,
} WallType;

typedef enum QuoridorState
{
    /// @brief Partie en cours.
    QUORIDOR_STATE_IN_PROGRESS,

    /// @brief Partie remportée par le joueur d'indice 0.
    QUORIDOR_STATE_P0_WON,

    /// @brief Partie remportée par le joueur d'indice 1.
    QUORIDOR_STATE_P1_WON,
} QuoridorState;

/// @brief Représente la position d'un pion sur le plateau de Quoridor.
/// La position (0,0) est en haut à gauche.
typedef struct QuoridorPos
{
    /// @brief Indice de la ligne (0 pour la ligne supérieure).
    int i;

    /// @brief Indice de la colonne (0 pour la colonne de gauche).
    int j;
} QuoridorPos;

/// @brief Représente l'état complet du jeu Quoridor.
typedef struct QuoridorCore
{
    /// @brief Taille de la grille (<= MAX_GRID_SIZE).
    int gridSize;

    /// @brief État actuel de la partie.
    QuoridorState state;

    /// @brief Murs verticaux placés sur le plateau.
    WallState vWalls[MAX_GRID_SIZE][MAX_GRID_SIZE];

    /// @brief Murs horizontaux placés sur le plateau.
    WallState hWalls[MAX_GRID_SIZE][MAX_GRID_SIZE];

    /// @brief Cases vers lesquelles le joueur courant peut se déplacer.
    /// Si isValid[i][j] vaut true, la case [i,j] est accessible.
    bool isValid[MAX_GRID_SIZE][MAX_GRID_SIZE];

    /// @brief Identifiant du joueur courant (0 ou 1).
    int playerID;

    /// @brief Position actuelle du pion de chaque joueur.
    QuoridorPos positions[2];

    /// @brief Nombre de murs restants pour chaque joueur.
    int wallCounts[2];
} QuoridorCore;

/// @brief Crée une instance du jeu Quoridor.
/// @return Un pointeur vers la structure créée.
QuoridorCore *QuoridorCore_create();

/// @brief Détruit une instance de Quoridor.
/// @param self Instance à détruire.
void QuoridorCore_destroy(QuoridorCore *self);

/// @brief Réinitialise une partie de Quoridor.
/// @param self Instance du jeu Quoridor.
/// @param gridSize Taille de la grille.
/// @param wallCount Nombre de murs initiaux par joueur.
/// @param firstPlayer Identifiant du premier joueur (0 ou 1).
void QuoridorCore_reset(QuoridorCore *self, int gridSize, int wallCount, int firstPlayer);

/// @brief Place 4 murs aléatoires sur le plateau.
/// À appeler juste après QuoridorCore_reset().
/// @param self Instance du jeu Quoridor.
void QuoridorCore_randomStart(QuoridorCore *self);

/// @brief Vérifie si le joueur courant peut poser un mur.
/// @param self Instance du jeu Quoridor.
/// @param type Type de mur (horizontal ou vertical).
/// @param i Ligne cible.
/// @param j Colonne cible.
/// @return true si l'action est possible, false sinon.
bool QuoridorCore_canPlayWall(QuoridorCore *self, WallType type, int i, int j);

/// @brief Vérifie si le joueur courant peut se déplacer vers une case.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne cible.
/// @param j Colonne cible.
/// @return true si le déplacement est possible, false sinon.
bool QuoridorCore_canMoveTo(QuoridorCore *self, int i, int j);

/// @brief Effectue le placement d'un mur.
/// Cette fonction ne vérifie pas la validité du coup.
/// Utiliser QuoridorCore_canPlayWall() au préalable.
/// @param self Instance du jeu Quoridor.
/// @param type Type du mur (horizontal ou vertical).
/// @param i Ligne cible.
/// @param j Colonne cible.
void QuoridorCore_playWall(QuoridorCore *self, WallType type, int i, int j);

/// @brief Effectue le déplacement du pion.
/// Cette fonction ne vérifie pas la validité du coup.
/// Utiliser QuoridorCore_canMoveTo() au préalable.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne cible.
/// @param j Colonne cible.
void QuoridorCore_moveTo(QuoridorCore *self, int i, int j);

/// @brief Affiche le plateau sur la sortie standard.
/// @param self Instance du jeu Quoridor.
void QuoridorCore_print(QuoridorCore *self);

typedef enum QuoridorAction
{
    /// @brief Action non définie.
    QUORIDOR_ACTION_UNDEFINED,

    /// @brief Déplacement.
    QUORIDOR_MOVE_TO,

    /// @brief Pose d'un mur horizontal.
    QUORIDOR_PLAY_HORIZONTAL_WALL,

    /// @brief Pose d'un mur vertical.
    QUORIDOR_PLAY_VERTICAL_WALL,
} QuoridorAction;

/// @brief Représente une action à effectuer par le joueur courant.
typedef struct QuoridorTurn
{
    /// @brief Type d'action (déplacement, mur horizontal ou vertical).
    QuoridorAction action;

    /// @brief Ligne cible.
    int i;

    /// @brief Colonne cible.
    int j;
} QuoridorTurn;

/// @brief Joue le tour du joueur courant.
/// Cette fonction ne vérifie pas la validité du coup.
/// Vérifier au préalable avec canPlayWall() ou canMoveTo().
/// @param self Instance du jeu Quoridor.
/// @param turn Action à effectuer.
void QuoridorCore_playTurn(QuoridorCore *self, QuoridorTurn turn);


/// @brief Vérifie s'il y a un mur horizontal au-dessus de [i,j].
/// Le bord supérieur est considéré comme un mur.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne.
/// @param j Colonne.
/// @return true s'il y a un mur, false sinon.
INLINE bool QuoridorCore_hasWallAbove(QuoridorCore *self, int i, int j)
{
    assert(0 <= i && i < self->gridSize);
    assert(0 <= j && j < self->gridSize);
    return (i > 0) ? self->hWalls[i - 1][j] : true;
}

/// @brief Vérifie s'il y a un mur horizontal en dessous de [i,j].
/// Le bord inférieur est considéré comme un mur.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne.
/// @param j Colonne.
/// @return true s'il y a un mur, false sinon.
INLINE bool QuoridorCore_hasWallBelow(QuoridorCore *self, int i, int j)
{
    assert(0 <= i && i < self->gridSize);
    assert(0 <= j && j < self->gridSize);
    return (i < self->gridSize - 1) ? self->hWalls[i][j] : true;
}

/// @brief Vérifie s'il y a un mur vertical à gauche de [i,j].
/// Le bord gauche est considéré comme un mur.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne.
/// @param j Colonne.
/// @return true s'il y a un mur, false sinon.
INLINE bool QuoridorCore_hasWallLeft(QuoridorCore *self, int i, int j)
{
    assert(0 <= i && i < self->gridSize);
    assert(0 <= j && j < self->gridSize);
    return (j > 0) ? self->vWalls[i][j - 1] : true;
}

/// @brief Vérifie s'il y a un mur vertical à droite de [i,j].
/// Le bord droit est considéré comme un mur.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne.
/// @param j Colonne.
/// @return true s'il y a un mur, false sinon.
INLINE bool QuoridorCore_hasWallRight(QuoridorCore *self, int i, int j)
{
    assert(0 <= i && i < self->gridSize);
    assert(0 <= j && j < self->gridSize);
    return (j < self->gridSize - 1) ? self->vWalls[i][j] : true;
}

/// @brief Définit la validité du déplacement vers [i,j].
/// Les indices peuvent être hors du plateau.
/// @param self Instance du jeu Quoridor.
/// @param i Ligne.
/// @param j Colonne.
/// @param isValid true si la case est atteignable, false sinon.
INLINE void QuoridorCore_setValidPosition(QuoridorCore *self, int i, int j, bool isValid)
{
    if (0 <= i && i < self->gridSize && 0 <= j && j < self->gridSize)
    {
        self->isValid[i][j] = isValid;
    }
}
