#ifndef __RAY_H__
#define __RAY_H__

#include <player.h>

#define FOV 90

struct ray {
    float x;
    float y;
};
typedef struct ray ray_t;

void move_ray(double direction, ray_t *ray, int *MAPDATA);

#endif