/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Structure représentant un atlas de textures.
typedef struct SpriteSheet
{
    SDL_Texture *texture;
    SDL_FRect *rects;
    int rectCount;
    bool pixelArt;
} SpriteSheet;

/// @brief Copie un sprite d'une sprite sheet vers la cible du moteur de rendu.
/// @param self la sprite sheet.
/// @param index indice du sprite à copier.
/// @param renderer le moteur de rendu.
/// @param dstRect le rectangle de destination sur le rendu.
/// @param angle l'angle de rotation, en degrés.
/// @param center le centre de rotation, ou NULL pour le centre de l'image.
/// @param flip flag indiquant quels retrounements (horizontal, vertical) sont appliqués à la copie.
void SpriteSheet_renderRotated(
    SpriteSheet *self, int index,
    SDL_Renderer *renderer, const SDL_FRect *dstRect,
    const double angle, const SDL_FPoint *center, const SDL_FlipMode flip);

/// @brief Modifie l'opacité d'une sprite sheet.
/// Cette fonction doit être appliquée avant une opération de copie sur le rendu
/// pour être prise en compte.
/// @param self la sprite sheet.
/// @param alpha l'opacité (0 pour transparant, 255 pour opaque).
void SpriteSheet_setOpacity(SpriteSheet *self, Uint8 alpha);

typedef struct SpriteSheetData SpriteSheetData;
typedef struct FontData FontData;

/// @brief Structure contenant l'ensemble des assets du jeu.
/// Cela correspond aux ressources utilisées (textures, fontes, musiques, son...)
typedef struct AssetManager
{
    int m_spriteCapacity;
    SpriteSheetData *m_spriteData;

    int m_fontCapacity;
    FontData *m_fontData;
} AssetManager;

/// @brief Crée le gestionnaire des assets du jeu.
/// @param spriteCapacity le nombre maximum de sprite sheets pouvant être ajoutées.
/// @param fontCapacity le nombre maximum de polices pouvant être ajoutées.
/// @return Le gestionnaire des assets du jeu.
AssetManager *AssetManager_create(int spriteCapacity, int fontCapacity);

/// @brief Détruit le gestionnaire des assets du jeu.
/// @param self les assets.
void AssetManager_destroy(AssetManager *self);

/// @brief Ajoute une sprite sheet au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
/// @param rectCount le nombre de sprites de la sprite sheet.
/// @param rectWidth la largeur d'un sprite.
/// @param rectHeight la hauteur d'un sprite.
/// @param pixelArt indique si la texture doit être rendue en pixel art.
void AssetManager_addSpriteSheet(
    AssetManager *self, int sheetID,
    const char *assetsPath, const char *fileName,
    int rectCount, int rectWidth, int rectHeight, bool pixelArt);

/// @brief Ajoute une police au gestionnaire des assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
/// @param assetsPath le chemin vers le dossier principal contenant les assets.
/// @param fileName le nom du fichier.
/// @param size la taille de la police.
void AssetManager_addFont(
    AssetManager *self, int fontID,
    const char *assetsPath, const char *fileName, float size);

/// @brief Renvoie une sprite sheet répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
/// @return La sprite sheet associée à l'identifiant sheetID;
SpriteSheet *AssetManager_getSpriteSheet(AssetManager *self, int sheetID);

/// @brief Renvoie une police répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
/// @return Le police associée à l'identifiant fontID.
TTF_Font *AssetManager_getFont(AssetManager *self, int fontID);

/// @brief Charge une sprite sheet répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param sheetID l'identifiant de la sprite sheet.
void AssetManager_loadSpriteSheet(AssetManager *self, int sheetID);

/// @brief Charge une police répertoriée dans le gestionnaire d'assets.
/// @param self le gestionnaire d'assets.
/// @param fontID l'identifiant de la police.
void AssetManager_loadFont(AssetManager *self, int fontID);

struct SpriteSheetData
{
    SpriteSheet *m_spriteSheet;
    char *m_fileName;
    int m_rectCount;
    int m_rectWidth;
    int m_rectHeight;
    int m_pixelArt;
};

struct FontData
{
    TTF_Font *m_font;
    char *m_fileName;
    void *m_buffer;
    SDL_IOStream *m_ioStream;
    float m_size;
};
