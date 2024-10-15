#include <ray.h>
#include <math.h>

#include <stdio.h>

#define WORLDSIZE (8*10)
#define MAPSIZE 8

void move_ray(double direction, ray_t *ray, int *MAPDATA)
{
	int hit = 0;

	// Move untill it hit a wall
	while(!hit)
	{
		ray[0].x += cos(direction);
		ray[0].y += sin(direction);


		if (MAPDATA[(int)(ray[0].y / WORLDSIZE)*MAPSIZE+(int)ray[0].x / WORLDSIZE] == 1)
		{
			hit = 1;
			break;
		}
	}

	// Round the rays position to an intager
	// Helps for straighter walls
	ray[0].x = (int)ray[0].x;
	ray[0].y = (int)ray[0].y;

	// Move out of the wall
	while(hit)
	{
		hit = 0;

		ray[0].x -= cos(direction) * 0.1f;
		ray[0].y -= sin(direction) * 0.1f;

		if (MAPDATA[(int)(ray[0].y / WORLDSIZE)*MAPSIZE+(int)ray[0].x / WORLDSIZE] == 1)
		{
			hit = 1;
		}
	}
}