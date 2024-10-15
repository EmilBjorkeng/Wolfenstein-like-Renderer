#include <player.h>
#include <math.h>
#include <stdio.h>

#define WORLDSIZE (8*10)
#define MAPSIZE 8

void move_player(player_t *player, int sign, int *MAPDATA, double DeltaTime)
{
    // Move X
    player[0].x += cos(player[0].angle) * SPEED*sign * DeltaTime;
    if (MAPDATA[(int)(player[0].y / WORLDSIZE)*MAPSIZE+(int)(player[0].x+PLAYERSIZE) / WORLDSIZE] == 1 ||
        MAPDATA[(int)(player[0].y / WORLDSIZE)*MAPSIZE+(int)(player[0].x-PLAYERSIZE) / WORLDSIZE] == 1)
    {
        player[0].x -= cos(player[0].angle) * SPEED*sign * DeltaTime;
    }

    //Move Y
    player[0].y += sin(player[0].angle) * SPEED*sign * DeltaTime;
    if (MAPDATA[(int)((player[0].y+PLAYERSIZE) / WORLDSIZE)*MAPSIZE+(int)player[0].x / WORLDSIZE] == 1 ||
        MAPDATA[(int)((player[0].y-PLAYERSIZE) / WORLDSIZE)*MAPSIZE+(int)player[0].x / WORLDSIZE] == 1)
    {
        player[0].y -= sin(player[0].angle) * SPEED*sign * DeltaTime;
    }
}

void turn_player(player_t *player, int sign, double DeltaTime)
{
    player[0].angle -= TURNSPEED*sign * DeltaTime / 10;
}