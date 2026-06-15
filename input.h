#pragma once // Не забываем защиту заголовочного файла!

#include <stdio.h>
#include <map>
#include <SDL2/SDL.h> // <--- ОБЯЗАТЕЛЬНО ДОБАВИТЬ

#include "config.h"
#include "render.h"

SDL_Point handleEvent(SDL_Event* e);