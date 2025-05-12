/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "engine/asset_manager.h"
#include "engine/common.h"

static void AssetManager_createIOStream(const char *fileName, SDL_IOStream **ioStream, void **buffer);
static void AssetManager_destroyIOStream(SDL_IOStream *ioStream, void *buffer);

static void SpriteSheetData_load(SpriteSheetData *self);
static void SpriteSheetData_clear(SpriteSheetData *self);

static void FontData_load(FontData *self);
static void FontData_clear(FontData *self);

static char *AssetManager_makePath(const char *assetsPath, const char *fileName);
SDL_Texture *AssetManager_loadTexture(SDL_Renderer *renderer, const char *fileName);

void SpriteSheet_renderRotated(
    SpriteSheet *self, int index,
    SDL_Renderer *renderer, const SDL_FRect *dstRect,
    const double angle, const SDL_FPoint *center, const SDL_FlipMode flip)
{
        assert(self && renderer);
        assert(index >= 0);
        index = index % self->rectCount;
        SDL_RenderTextureRotated(renderer, self->texture, self->rects + index, dstRect, angle, center, flip);
}

void SpriteSheet_setOpacity(SpriteSheet *self, Uint8 alpha)
{
    assert(SDL_SetTextureBlendMode(self->texture, SDL_BLENDMODE_BLEND));
    assert(SDL_SetTextureAlphaMod(self->texture, alpha));
}

AssetManager *AssetManager_create(int spriteCapacity, int fontCapacity)
{
    AssetManager *self = (AssetManager *)calloc(1, sizeof(AssetManager));
    AssertNew(self);

    self->m_spriteCapacity = spriteCapacity;
    self->m_fontCapacity = fontCapacity;

    self->m_spriteData = (SpriteSheetData *)calloc(spriteCapacity, sizeof(SpriteSheetData));
    AssertNew(self->m_spriteData);

    self->m_fontData = (FontData *)calloc(fontCapacity, sizeof(FontData));
    AssertNew(self->m_fontData);

    return self;
}

void AssetManager_destroy(AssetManager *self)
{
    if (!self) return;

    if (self->m_spriteData)
    {
        // Libère les spriteSheet
        for (int i = 0; i < self->m_spriteCapacity; i++)
        {
            SpriteSheetData_clear(self->m_spriteData + i);
        }
        free(self->m_spriteData);
    }

    if (self->m_fontData)
    {
       // Libère les fonts
       for (int i = 0; i < self->m_fontCapacity; i++)
       {
           FontData_clear(self->m_fontData + i);
       }
       free(self->m_fontData);
    }

    free(self);
}

static char *AssetManager_makePath(const char *assetsPath, const char *fileName)
{
    assert(assetsPath && "The assets path is not valid");
    assert(fileName && "The file name is not valid");

    size_t assetPathLen = strlen(assetsPath);
    size_t fileNameLen = strlen(fileName);
    size_t pathSize = assetPathLen + fileNameLen + 1;
    char *fullPath = (char *)calloc(pathSize + 1, sizeof(char));
    AssertNew(fullPath);
    Memcpy(fullPath, pathSize, assetsPath, assetPathLen);
    Memcpy(fullPath + assetPathLen, fileNameLen, fileName, fileNameLen);
    return fullPath;
}

void AssetManager_addSpriteSheet(
    AssetManager *self, int sheetID,
    const char *assetsPath, const char *fileName,
    int rectCount, int rectWidth, int rectHeight, bool pixelArt)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= sheetID && sheetID < self->m_spriteCapacity && "The sheetID is not valid");

    SpriteSheetData *spriteData = &(self->m_spriteData[sheetID]);
    if (spriteData->m_fileName)
    {
        printf("ERROR - Add sprite sheet %s%s\n", assetsPath, fileName);
        printf("      - The sprite sheet with ID %d already exists\n", sheetID);
        assert(false && "The sheetID is already used");
        return;
    }
    spriteData->m_fileName = AssetManager_makePath(assetsPath, fileName);
    spriteData->m_rectCount = rectCount;
    spriteData->m_rectWidth = rectWidth;
    spriteData->m_rectHeight = rectHeight;
    spriteData->m_pixelArt = pixelArt;
}

void AssetManager_addFont(
    AssetManager *self, int fontID,
    const char *assetsPath, const char *fileName, float size)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= fontID && fontID < self->m_fontCapacity && "The fontID is not valid");

    FontData *fontData = &(self->m_fontData[fontID]);
    if (fontData->m_fileName)
    {
        printf("ERROR - Add font %s%s\n", assetsPath, fileName);
        printf("      - The font with ID %d already exists\n", fontID);
        assert(false && "The fontID is already used");
        return;
    }
    fontData->m_fileName = AssetManager_makePath(assetsPath, fileName);
    fontData->m_size = size;
}

SpriteSheet *AssetManager_getSpriteSheet(AssetManager *self, int sheetID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= sheetID && sheetID < self->m_spriteCapacity && "The sheetID is not valid");

    SpriteSheetData *spriteData = &(self->m_spriteData[sheetID]);
    if (spriteData->m_spriteSheet) return spriteData->m_spriteSheet;

    if (spriteData->m_fileName == NULL)
    {
        printf("ERROR - No sprite sheet with ID %d\n", sheetID);
        assert(false && "No sprite sheet with this sheetID");
        return NULL;
    }

    SpriteSheetData_load(spriteData);
    return spriteData->m_spriteSheet;
}

TTF_Font *AssetManager_getFont(AssetManager *self, int fontID)
{
    assert(self && "The AssetManager must be created");
    assert(0 <= fontID && fontID < self->m_fontCapacity && "The fontID is not valid");

    FontData *fontData = &(self->m_fontData[fontID]);
    if (fontData->m_font) return fontData->m_font;

    if (fontData->m_fileName == NULL)
    {
        printf("ERROR - No font with ID %d\n", fontID);
        assert(false && "No font with this fontID");
        return NULL;
    }

    FontData_load(fontData);
    return fontData->m_font;
}

void AssetManager_loadSpriteSheet(AssetManager *self, int sheetID)
{
    SpriteSheet *spriteSheet = AssetManager_getSpriteSheet(self, sheetID);
    assert(spriteSheet);
}

void AssetManager_loadFont(AssetManager *self, int fontID)
{
    TTF_Font *font = AssetManager_getFont(self, fontID);
    assert(font);
}

static void SpriteSheetData_load(SpriteSheetData *self)
{
    SpriteSheet *spriteSheet = (SpriteSheet *)calloc(1, sizeof(SpriteSheet));
    AssertNew(spriteSheet);

    self->m_spriteSheet = spriteSheet;

    void *buffer = NULL;
    SDL_IOStream *ioStream = NULL;
    AssetManager_createIOStream(self->m_fileName, &ioStream, &buffer);

    spriteSheet->pixelArt = self->m_pixelArt;
    spriteSheet->texture = IMG_LoadTexture_IO(g_renderer, ioStream, false);
    if (spriteSheet->texture == NULL)
    {
        printf("ERROR - Loading m_spriteSheet %s\n", self->m_fileName);
        printf("      - %s\n", SDL_GetError());
        assert(false);
        abort();
    }
    AssetManager_destroyIOStream(ioStream, buffer);
    ioStream = NULL; buffer = NULL;

    int rectCount = self->m_rectCount;
    spriteSheet->rectCount = rectCount;
    spriteSheet->rects = (SDL_FRect *)calloc(rectCount, sizeof(SDL_FRect));
    AssertNew(spriteSheet->rects);

    int w = spriteSheet->texture->w;
    int x = 0, y = 0;
    for (int i = 0; i < rectCount; i++)
    {
        spriteSheet->rects[i].w = (float)self->m_rectWidth;
        spriteSheet->rects[i].h = (float)self->m_rectHeight;
        spriteSheet->rects[i].x = (float)x;
        spriteSheet->rects[i].y = (float)y;

        x += self->m_rectWidth;
        if (x > w - self->m_rectWidth)
        {
            x = 0;
            y += self->m_rectHeight;
        }
    }

    if (spriteSheet->pixelArt)
        SDL_SetTextureScaleMode(spriteSheet->texture, SDL_SCALEMODE_NEAREST);
}

static void SpriteSheetData_clear(SpriteSheetData *self)
{
    if (self->m_spriteSheet)
    {
        free(self->m_spriteSheet->rects);
        if (self->m_spriteSheet->texture)
            SDL_DestroyTexture(self->m_spriteSheet->texture);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(SpriteSheetData));
}

static void FontData_load(FontData *self)
{
    AssetManager_createIOStream(self->m_fileName, &self->m_ioStream, &self->m_buffer);

    self->m_font = TTF_OpenFontIO(self->m_ioStream, false, self->m_size);
    if (self->m_font == NULL)
    {
        printf("ERROR - Loading font %s\n", self->m_fileName);
        printf("      - %s\n", SDL_GetError());
        assert(false);
        abort();
    }
}

static void FontData_clear(FontData *self)
{
    if (self->m_font)
    {
        TTF_CloseFont(self->m_font);
        AssetManager_destroyIOStream(self->m_ioStream, self->m_buffer);
    }
    free(self->m_fileName);
    memset(self, 0, sizeof(FontData));
}

static void AssetManager_retriveMem(void *memory, size_t size)
{
    char *buffer = (char *)memory;
    for (size_t i = size - 1; i > 0; i--)
    {
        buffer[i] = 0x73 * (buffer[i] + 0x37);
        buffer[i] ^= buffer[i - 1];
    }
    buffer[0] = 0x73 * (buffer[0] + 0x37);
    buffer[0] ^= 0x73;
}

void AssetManager_createIOStream(const char *fileName, SDL_IOStream **ioStream, void **buffer)
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL)
    {
        printf("ERROR - The file %s cannot be opened\n", fileName);
        assert(false);
        abort();
    }

    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file);
    rewind(file);
    uint8_t *mem = (unsigned char *)calloc(size, 1);
    AssertNew(mem);

    *buffer = (void *)mem;

    size_t freadCount = fread(mem, 1, size, file);
    assert(freadCount == size);
    fclose(file); file = NULL;

    if (size > 2 && mem[0] == (uint8_t)0x0B && mem[1] == (uint8_t)0xF7)
    {
        // Utilisation du magic number 0x0BF7 pour les fichiers obsfusqués
        mem += 2;
        size -= 2;
        AssetManager_retriveMem((void *)mem, size);
    }

    *ioStream = SDL_IOFromConstMem(mem, (int)size);

    if (*ioStream == NULL)
    {
        printf("ERROR - Loading IOStream %s\n", fileName);
        printf("      - %s\n", SDL_GetError());
        assert(false);
        abort();
    }
}

void AssetManager_destroyIOStream(SDL_IOStream *ioStream, void *buffer)
{
    if (ioStream) SDL_CloseIO(ioStream);
    free(buffer);
}

SDL_Texture *AssetManager_loadTexture(SDL_Renderer *renderer, const char *fileName)
{
    void *buffer = NULL;
    SDL_IOStream *ioStream = NULL;
    AssetManager_createIOStream(fileName, &ioStream, &buffer);

    SDL_Texture *texPtr = IMG_LoadTexture_IO(renderer, ioStream, false);
    if (texPtr == NULL)
    {
        printf("ERROR - Loading texture %s\n", fileName);
        printf("      - %s\n", SDL_GetError());
        assert(false);
        abort();
    }

    AssetManager_destroyIOStream(ioStream, buffer);

    return texPtr;
}
