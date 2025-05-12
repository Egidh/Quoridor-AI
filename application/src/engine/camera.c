/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "engine/camera.h"

Camera *Camera_create()
{
    Camera *self = (Camera *)calloc(1, sizeof(Camera));
    AssertNew(self);

    float worldW = 16.0f;
    float worldH = 9.0f;

    self->m_worldView = AABB_set(0.0f, 0.0f, worldW, worldH);
    self->m_viewport.h = worldH;
    self->m_viewport.w = worldW;

    return self;
}

void Camera_destroy(Camera *self)
{
    if (!self) return;
    free(self);
}

void Camera_updateViewport(Camera *self, SDL_Renderer *renderer)
{
    assert(self && "The Camera must be created");
    assert(renderer && "The SDL_Renderer must be created");

    int outW = 0, outH = 0;
    bool success = SDL_GetRenderOutputSize(renderer, &outW, &outH);
    assert(success);

    const float worldW = self->m_worldView.upper.x - self->m_worldView.lower.x;
    const float worldH = self->m_worldView.upper.y - self->m_worldView.lower.y;
    const float worldAspectRatio = worldW / worldH;
    const float outputAspectRatio = (float)outW / (float)outH;

    self->m_viewport.x = 0.f;
    self->m_viewport.y = 0.f;
    if (outputAspectRatio >= worldAspectRatio)
    {
        // Output plus large
        self->m_viewport.w = worldAspectRatio * (float)outH;
        self->m_viewport.h = (float)outH;
        self->m_viewport.x = 0.5f * (outW - self->m_viewport.w);
    }
    else
    {
        // Output plus haut
        self->m_viewport.w = (float)outW;
        self->m_viewport.h = (float)outW / worldAspectRatio;
        self->m_viewport.y = 0.5f * (outH - self->m_viewport.h);
    }


    SDL_Rect viewport = { 0 };
    viewport.x = (int)roundf(self->m_viewport.x);
    viewport.y = (int)roundf(self->m_viewport.y);
    viewport.w = (int)roundf(self->m_viewport.w);
    viewport.h = (int)roundf(self->m_viewport.h);

    SDL_SetRenderViewport(renderer, &viewport);
}

float Camera_worldToViewX(Camera *self, float positionX)
{
    assert(self && "The Camera must be created");
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    float scale = self->m_viewport.w / w;
    return (positionX - self->m_worldView.lower.x) * scale;
}

float Camera_worldToViewY(Camera *self, float positionY)
{
    assert(self && "The Camera must be created");
    float h = self->m_worldView.upper.y - self->m_worldView.lower.y;
    float scale = self->m_viewport.h / h;
    return (self->m_worldView.upper.y - positionY) * scale;
}

void Camera_worldToView(Camera *self, Vec2 position, float *x, float *y)
{
    assert(self && "The Camera must be created");
    assert(x && y);
    *x = Camera_worldToViewX(self, position.x);
    *y = Camera_worldToViewY(self, position.y);
}

void Camera_viewToWorld(Camera *self, float x, float y, Vec2 *position)
{
    assert(self && "The Camera must be created");
    y = self->m_viewport.h - y;
    float ratioX = (x - self->m_viewport.x) / self->m_viewport.w;
    float ratioY = (y - self->m_viewport.y) / self->m_viewport.h;
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    float h = self->m_worldView.upper.y - self->m_worldView.lower.y;
    position->x = self->m_worldView.lower.x + ratioX * w;
    position->y = self->m_worldView.lower.y + ratioY * h;
}

void Camera_translateWorldView(Camera *self, Vec2 displacement)
{
    assert(self && "The Camera must be created");
    AABB_translate(&self->m_worldView, displacement);
}

float Camera_getWidth(Camera *self)
{
    assert(self && "The Camera must be created");
    return self->m_viewport.w;
}

float Camera_getHeight(Camera *self)
{
    assert(self && "The Camera must be created");
    return self->m_viewport.h;
}

float Camera_getWorldToViewScale(Camera *self)
{
    assert(self && "The Camera must be created");
    float w = self->m_worldView.upper.x - self->m_worldView.lower.x;
    return self->m_viewport.w / w;
}
