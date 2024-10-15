#ifndef __TIME_H__
#define __TIME_H__

#include <SDL2/SDL_ttf.h>

float DeltaTime;
float fps;
float MSPerFrame;

uint64_t PerfFrequency;
uint64_t FPSCounter;

void deltaTimeGetPerformance();
void deltaTime();
void drawFPS(SDL_Renderer *Renderer, TTF_Font *Font);

#endif