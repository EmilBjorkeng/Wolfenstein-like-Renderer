#include <time.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void deltaTimeGetPerformance()
{
    PerfFrequency = SDL_GetPerformanceFrequency();
    FPSCounter = SDL_GetPerformanceCounter();
}

void deltaTime()
{
    uint64_t EndCounter = SDL_GetPerformanceCounter();
    uint64_t ElapsedTime = EndCounter - FPSCounter;
    DeltaTime = (float)ElapsedTime / (float)PerfFrequency;

    fps = (float)PerfFrequency / (float)ElapsedTime;
    MSPerFrame = 1000.0f * DeltaTime;
}

void drawFPS(SDL_Renderer *Renderer, TTF_Font *Font)
{
    char text[20];
    sprintf(text, "FPS: %d", (int)fps);

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Font, text, (SDL_Color){50, 180, 70});
    SDL_Texture *message = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);

    SDL_Rect messageRect = (SDL_Rect){5, 5, surfaceMessage->w, surfaceMessage->h};
    SDL_RenderCopy(Renderer, message, NULL, &messageRect);
}