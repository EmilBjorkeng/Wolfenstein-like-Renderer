#ifndef __PLAYER_H__
#define __PLAYER_H__

#define SPEED 100.0f
#define TURNSPEED 18.0f
#define PLAYERSIZE 10

struct player {
    float x;
    float y;
    float angle;
};
typedef struct player player_t;

void move_player(player_t *player, int sign, int *MAPDATA, double DeltaTime);
void turn_player(player_t *player, int sign, double DeltaTime);

#endif