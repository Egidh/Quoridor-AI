/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "engine/common.h"

Timer *g_time = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Window *g_window = NULL;

SDL_AppResult Game_init(int sdlFlags, int mixFlags)
{
    // Initialise la SDL2
    if (!SDL_Init(sdlFlags))
    {
        printf("ERROR - SDL_Init %s\n", SDL_GetError());
        assert(false);
        return SDL_APP_FAILURE;
    }

    // Initialise la SDL2 TTF
    if (!TTF_Init())
    {
        printf("ERROR - TTF_Init %s\n", SDL_GetError());
        assert(false);
        return SDL_APP_FAILURE;
    }

    // Crée le temps global du jeu
    g_time = Timer_create();
    AssertNew(g_time);

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_createWindow(int width, int height, const char *title, Uint32 flags)
{
    assert(g_window == NULL && "The window is already created");

    g_window = SDL_CreateWindow(title, width, height, flags);
    if (!g_window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_createRenderer(int width, int height)
{
    assert(g_renderer == NULL && "The renderer is already created");
    assert(g_window);

    printf("INFO - Available rendering engines:\n");
    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++)
    {
        printf("     - \"%s\"\n", SDL_GetRenderDriver(i));
    }

    //g_renderer = SDL_CreateRenderer(g_window, "direct3d11");
    //g_renderer = SDL_CreateRenderer(g_window, "direct3d12");
    //g_renderer = SDL_CreateRenderer(g_window, "opengl");
    //g_renderer = SDL_CreateRenderer(g_window, "opengl");
    g_renderer = SDL_CreateRenderer(g_window, NULL);

    printf("INFO - Selected renderer: \"%s\"\n", SDL_GetRendererName(g_renderer));

    if (!g_renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false);
        return SDL_APP_FAILURE;
    }

    bool success = true;
    success = SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    assert(success);

    if (!SDL_SetRenderVSync(g_renderer, 1))
    {
        printf("ERROR - Render VSync %s\n", SDL_GetError());
    }

    return SDL_APP_CONTINUE;
}

void Game_destroyRenderer()
{
    if (!g_renderer) return;
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;
}

void Game_destroyWindow()
{
    if (!g_window) return;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
}

void Game_quit()
{
    assert(g_window == NULL);
    assert(g_renderer == NULL);

    Timer_destroy(g_time);
    g_time = NULL;

    TTF_Quit();
    SDL_Quit();
}

void Game_setRenderDrawColor(SDL_Color color, Uint8 alpha)
{
    assert(g_renderer && "The renderer must be created");
    SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, alpha);
}

void Memcpy(void *const dst, size_t dstSize, const void *src, size_t srcSize)
{
#ifdef _MSC_VER
    memcpy_s(dst, dstSize, src, srcSize);
#else
    memcpy(dst, src, (srcSize < dstSize) ? srcSize : dstSize);
#endif
}

char *Strdup(const char *src)
{
    size_t size = strlen(src);
    char *dst = (char *)calloc(size + 1, sizeof(char));
    AssertNew(dst);
    Memcpy(dst, size, src, size);
    return dst;
}
