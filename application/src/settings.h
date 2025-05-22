/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#ifdef _MSC_VER
#define INLINE inline
#else
#define INLINE static inline
#endif

#define AssertNew(ptr)     \
    {                      \
        if (ptr == NULL)   \
        {                  \
            assert(false); \
            abort();       \
        }                  \
    }

#define HD_WIDTH 1280
#define HD_HEIGHT 720
#define FHD_WIDTH 1920
#define FHD_HEIGHT 1080

#define FILE_TO_SAVE_GAME "../../data/test.txt"

#define DEBUG 1
