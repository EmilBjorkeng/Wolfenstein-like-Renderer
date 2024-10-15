#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <time.h>
#include <input.h>
#include <player.h>
#include <ray.h>

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define PI 3.14159265359f
#define DEG2RAD(_d) ((_d) * (PI / 180.0f))
#define RAD2DEG(_d) ((_d) * (180.0f / PI))

#define TITLE "3D Renderer"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define min(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })
#define clamp(_x, _mi, _ma) (min(max(_x, _mi), _ma))

#define POW2(_a) ((_a) * (_a))
#define ABS(_a) ((_a) > 0 ? (_a) : -(_a))

#define FLOOR(a) ({__typeof__(a) _a = (a); (int)_a <= _a ? (__typeof__(_a))(int)_a : (__typeof__(_a))((int)_a - 1); })
#define CEIL(a) ({__typeof__(a) _a = (a); (int)_a >= _a ? (__typeof__(_a))(int)_a : (__typeof__(_a))((int)_a + 1); })

#define ROUND(_a) ((_a) - (int)(_a) >= 0.5 ? CEIL(_a) : FLOOR(_a))

#define BLOCKSIZE 80
#define MAPSIZE 8
static int MAPDATA[MAPSIZE * MAPSIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

int pixels[WINDOW_WIDTH * WINDOW_HEIGHT];

SDL_Window *Window;
SDL_Renderer *Renderer;
SDL_Event WindowEvent;
TTF_Font *Font;
SDL_Texture *texture;

int main(int argc, char *argv[])
{
    ASSERT(
        !SDL_Init(SDL_INIT_EVENTS),
        "Initialization of SDL failed: %s\n",
        SDL_GetError());

    ASSERT(
        !TTF_Init(),
        "Initialization of TTF failed: %s\n",
        SDL_GetError());

    Window = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI);
    ASSERT(Window, "Window creation failed: %s\n", SDL_GetError());

    Renderer = SDL_CreateRenderer(
        Window,
        -1,
        SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC);
    ASSERT(Renderer, "Creation of the renderer failed: %s\n", SDL_GetError());

    Font = TTF_OpenFont(
        "assets/Montserrat-Medium.ttf",
        32);
    ASSERT(Font, "Cound not load the font: %s\n", SDL_GetError());

    SDL_Surface *brickSurface = IMG_Load("assets/brick.jpg");
    ASSERT(
        brickSurface,
        "Failed to load the brick surface %s\n",
        SDL_GetError());

    player_t player;

    player.x = BLOCKSIZE*MAPSIZE/2+10;
    player.y = BLOCKSIZE*MAPSIZE/2+10;
    player.angle = 0;

    while (1)
    {
        deltaTimeGetPerformance();

        if (SDL_PollEvent(&WindowEvent))
        {
            if (WindowEvent.type == SDL_QUIT)
            { break; }
        }

        //
        // Input
        //

        const uint8_t *keyboard_state_array = SDL_GetKeyboardState(NULL);
        if (KEY_PRESSED(KEY_W))
        {
            move_player(&player, 1, MAPDATA, DeltaTime);
        }
        if (KEY_PRESSED(KEY_S))
        {
            move_player(&player, -1, MAPDATA, DeltaTime);
        }
        if (KEY_PRESSED(KEY_A))
        {
            turn_player(&player, 1, DeltaTime);
        }
        if (KEY_PRESSED(KEY_D))
        {
            turn_player(&player, -1, DeltaTime);
        }
        // Keep playerAngle between 360 and 0 degrees
        if (RAD2DEG(player.angle) > 360) player.angle -= DEG2RAD(360);
        else if (RAD2DEG(player.angle) < 0) player.angle += DEG2RAD(360);

        //BREAKPOINT
        if (KEY_PRESSED(SDL_SCANCODE_H))
        {
            printf("BREAK");
        }

        //
        // Draw
        //

        //SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0xFF);
        //SDL_RenderClear(Renderer);

        texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

        for (int h = 0; h < WINDOW_HEIGHT; h++)
        {
            for (int w = 0; w < WINDOW_WIDTH; w++)
            {
                pixels[(h * WINDOW_WIDTH) + w] = 0x000000FF;
            }
        }

        //
        // Raycast
        //
        int DV = WINDOW_WIDTH / tan(FOV / 2);
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            // Spawn a ray at the players position
            ray_t ray;
            ray.x = player.x;
            ray.y = player.y;

            double direction = player.angle + atan((float)(x - WINDOW_WIDTH/2) / DV);
            move_ray(direction, &ray, MAPDATA);

            double rayLength = sqrt(POW2(ABS((player.y - ray.y))) + POW2(ABS((player.x - ray.x))));
            rayLength *= cos(direction - player.angle); // lens correction
            int height = (DV / rayLength) * BLOCKSIZE/2;

            int wallTop = max((height*2 - WINDOW_HEIGHT) / 2, 0);
            for (int h = wallTop; h < (height*2) - wallTop; h++)
            {
                int tx, ty;

                int xWall = MAPDATA[(int)(ray.y / BLOCKSIZE)*MAPSIZE+(int)(ray.x+1) / BLOCKSIZE] == 1 ||
                           MAPDATA[(int)(ray.y / BLOCKSIZE)*MAPSIZE+(int)(ray.x-1) / BLOCKSIZE] == 1;

                // X Wall
                if (xWall)
                {
                    int xper = fmod(ray.y, BLOCKSIZE) / (BLOCKSIZE-1) * 100;
                    tx = xper * brickSurface->w / 100;
                    ty = h * brickSurface->h / (height*2);
                }
                // Y Wall
                else
                {
                    int xper = fmod(ray.x, BLOCKSIZE) / BLOCKSIZE * 100;
                    tx = xper * (brickSurface->w+1) / 100;
                    ty = h * brickSurface->h / (height*2);
                }

                Uint8 *p = (Uint8 *)brickSurface->pixels + (ty * brickSurface->pitch) + (tx * brickSurface->format->BytesPerPixel);
                uint32_t colour = p[0]*16777216 + p[1]*65536 + p[2]*256 + 255;

                // Darken walls on the X axis
                if (xWall)
                {
                    const uint32_t
                        r = (colour & 0xFF000000),
                        g = (colour & 0x00FF0000),
                        b = (colour & 0x0000FF00);

                    colour = ((uint32_t)(r * 0.70f) & 0xFF000000) | ((uint32_t)(g * 0.70f) & 0xFF0000) | ((uint32_t)(b * 0.70f) & 0xFF00) | 0xFF;
                }

                int y = clamp((WINDOW_HEIGHT / 2 - height) + h, 0, WINDOW_HEIGHT-1);
                pixels[(y * WINDOW_WIDTH) + x] = colour;
            }
        }

        SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
        SDL_RenderCopyEx(Renderer, texture, NULL, NULL, 0.0, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(texture);

        drawFPS(Renderer, Font);

        SDL_RenderPresent(Renderer);
        deltaTime();
    }
    SDL_FreeSurface(brickSurface);

    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    return 0;
}