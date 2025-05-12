/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/input.h"
#include "engine/common.h"
#include "engine/math.h"

#define TRIGGER_MAX_VALUE 32767

Input *Input_create()
{
    Input *self = (Input *)calloc(1, sizeof(Input));
    AssertNew(self);

    int playerID = 0;
    int gamepadCount = 0;
    SDL_JoystickID *joysticks = SDL_GetGamepads(&gamepadCount);
    if (joysticks)
    {
        for (int i = 0; i < gamepadCount; i++)
        {
            SDL_Gamepad *gamepad = SDL_OpenGamepad(joysticks[i]);
            SDL_SetGamepadPlayerIndex(gamepad, playerID);
            playerID += (playerID < MAX_PLAYER_COUNT - 1) ? 1 : 0;
        }
        SDL_free(joysticks);
    }

    return self;
}

void Input_destroy(Input *self)
{
    if (!self) return;
    free(self);
}

void Input_update(Input *self)
{
    assert(self);
    PlayerInput *playerInput = NULL;
    int playerID = 0;

    self->quitPressed = false;
    self->validatePressed = false;


    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        playerInput = &(self->players[i]);
        playerInput->validatePressed = false;
    }

    SDL_Event event = { 0 };
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            self->quitPressed = true;
            break;

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            playerID = SDL_GetGamepadPlayerIndexForID(event.gbutton.which);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerButtonDown(self, playerInput, event.gbutton.button);
            break;

        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            playerID = SDL_GetGamepadPlayerIndexForID(event.gbutton.which);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerButtonUp(self, playerInput, event.gbutton.button);
            break;

        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            playerID = SDL_GetGamepadPlayerIndexForID(event.gbutton.which);
            assert(0 <= playerID && playerID < MAX_PLAYER_COUNT);
            playerInput = &(self->players[playerID]);
            Input_updateControllerAxisMotion(self, playerInput, event.gaxis.axis, event.gaxis.value);
            break;

        case SDL_EVENT_KEY_DOWN:
            playerInput = &(self->players[0]);
            Input_updateKeyDown(self, playerInput, event.key.repeat, event.key.scancode);
            break;

        case SDL_EVENT_KEY_UP:
            playerInput = &(self->players[0]);
            Input_updateKeyUp(self, playerInput, event.key.repeat, event.key.scancode);
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            Input_updateMouseButtonDown(self, event.button.button);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            Input_updateMouseButtonUp(self, event.button.button);
            break;
        }
    }

    float windowX, windowY;
    //SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&windowX, &windowY);
    (void)SDL_GetMouseState(&windowX, &windowY);
    SDL_RenderCoordinatesFromWindow(g_renderer, windowX, windowY, &(self->mousePos.x), &(self->mousePos.y));

    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        playerInput = &(self->players[i]);

        if (playerInput->validatePressed) self->validatePressed = true;
    }
}

void Input_updateControllerButtonDown(Input *self, PlayerInput *playerInput, int button)
{
    switch (button)
    {
    case SDL_GAMEPAD_BUTTON_SOUTH:
        playerInput->validatePressed = true;
        break;
    case SDL_GAMEPAD_BUTTON_WEST:
        break;
    case SDL_GAMEPAD_BUTTON_EAST:
        break;
    case SDL_GAMEPAD_BUTTON_START:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        break;
    default: break;
    }
}

void Input_updateControllerButtonUp(Input *self, PlayerInput *playerInput, int button)
{
    switch (button)
    {
    case SDL_GAMEPAD_BUTTON_SOUTH:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        break;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        break;
    default: break;
    }
}

void Input_updateControllerAxisMotion(Input *self, PlayerInput *playerInput, int axis, Sint16 value)
{
    switch (axis)
    {
    case SDL_GAMEPAD_AXIS_LEFTX:
        break;

    case SDL_GAMEPAD_AXIS_LEFTY:
        break;

    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
        break;

    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
        break;

    default: break;
    }
}

void Input_updateKeyDown(Input *self, PlayerInput *playerInput, bool repeat, int scancode)
{
    if (repeat) return;

    switch (scancode)
    {
    case SDL_SCANCODE_ESCAPE:
        self->quitPressed = true;
        break;
    case SDL_SCANCODE_UP:
        break;
    case SDL_SCANCODE_DOWN:
        break;
    case SDL_SCANCODE_LEFT:
        break;
    case SDL_SCANCODE_RIGHT:
        break;
    case SDL_SCANCODE_SPACE:
        break;
    case SDL_SCANCODE_RETURN:
        break;
    case SDL_SCANCODE_BACKSPACE:
        break;
    default: break;
    }
}

void Input_updateKeyUp(Input *self, PlayerInput *playerInput, bool repeat, int scancode)
{
    if (repeat) return;

    switch (scancode)
    {
    case SDL_SCANCODE_SPACE:
        break;
    default: break;
    }
}

void Input_updateMouseButtonDown(Input *self, int button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        self->validatePressed = true;
        break;
    case SDL_BUTTON_MIDDLE:
        break;
    case SDL_BUTTON_RIGHT:
        break;
    default: break;
    }
}

void Input_updateMouseButtonUp(Input *self, int button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        break;
    case SDL_BUTTON_MIDDLE:
        break;
    case SDL_BUTTON_RIGHT:
        break;
    default: break;
    }
}
