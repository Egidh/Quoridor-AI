/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "engine/timer.h"

Timer* Timer_create()
{
    Timer *self = (Timer*)calloc(1, sizeof(Timer));
    AssertNew(self);

    self->m_currentTime = 0;
    self->m_previousTime = 0;

    self->m_delta = 0;
    self->m_unscaledDelta = 0;
    self->m_elapsed = 0;
    self->m_unscaledElapsed = 0;

    self->m_maxDelta = 100;
    self->m_scale = 1.0f;

    return self;
}

void Timer_destroy(Timer* self)
{
    if (!self) return;
    free(self);
}

void Timer_start(Timer* self)
{
    assert(self && "The Timer must be created");
    self->m_currentTime = SDL_GetTicks();
    self->m_previousTime = SDL_GetTicks();
    self->m_delta = 0;
}

void Timer_update(Timer* self)
{
    assert(self && "The Timer must be created");
    self->m_previousTime = self->m_currentTime;
    self->m_currentTime = SDL_GetTicks();

    self->m_unscaledDelta = self->m_currentTime - self->m_previousTime;
    if (self->m_unscaledDelta > self->m_maxDelta)
    {
        self->m_unscaledDelta = self->m_maxDelta;
    }
    self->m_delta = (Uint64)(self->m_scale * (double)self->m_unscaledDelta);

    self->m_unscaledElapsed += self->m_unscaledDelta;
    self->m_elapsed += self->m_delta;
}