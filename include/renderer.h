#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

struct point_3D{

	float x, y, z;
	
	float screenX, screenY;

};

struct object_3D{

	float x, y, z;

	float w, h, d;

	int points_size;
	struct point_3D points[8];
};

//create
struct object_3D create_object_3D(float _x, float _y, float _z, float _w, float _h, float _d);

//modify
void rotate_x(struct object_3D* obj, float theta);
void rotate_y(struct object_3D* obj, float theta);
void rotate_z(struct object_3D* obj, float theta);

//display
void project_object(struct object_3D* obj, int resX, int resY);
void render_object(SDL_Renderer* renderer, struct object_3D* obj);





