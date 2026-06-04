#include "renderer.h"


struct object_3D create_object_3D(float _x, float _y, float _z, float _w, float _h, float _d){

	struct object_3D temp;

	temp.x = _x;// + (resX / 2);
	temp.y = _y;// + (resY / 2);
	temp.z = _z;
	
	temp.w = _w;
	temp.h = _h;
	temp.d = _d;

	float hw = _w / 2;
	float hh = _h / 2;
	float hd = _d / 2;

	temp.points[0] = (struct point_3D){-hw, -hh, -hd}; //top left front point
	temp.points[1] = (struct point_3D){ hw, -hh, -hd}; //top right front point
	temp.points[2] = (struct point_3D){ hw, -hh,  hd}; //top right deep point
	temp.points[3] = (struct point_3D){-hw, -hh,  hd}; //top left deep point
	temp.points[4] = (struct point_3D){-hw,  hh, -hd}; //bottom left front point
	temp.points[5] = (struct point_3D){ hw,  hh, -hd}; //bottom right front point
	temp.points[6] = (struct point_3D){ hw,  hh,  hd}; //bottom right deep point
	temp.points[7] = (struct point_3D){-hw,  hh,  hd}; //bottom left deep point
	
	temp.points_size = sizeof(temp.points) / sizeof(temp.points[0]);
	temp.show_axis = 1;

	temp.axis[0] = (struct point_3D){0, 0, 0};
	temp.axis[1] = (struct point_3D){_w, 0, 0};
	temp.axis[2] = (struct point_3D){0, -_h, 0};
	temp.axis[3] = (struct point_3D){0, 0, _d};

	return temp;
};

void rotate_x(struct object_3D* obj, float theta){

	float cosT = cos(theta);
	float sinT = sin(theta);

	for(int i = 0; i < obj->points_size; i++){

		float y = obj->points[i].y;
		float z = obj->points[i].z;

		obj->points[i].y = y * cosT - z * sinT;
		obj->points[i].z = y * sinT + z * cosT;
	}

	for(int i = 1; i < 4; i++){

		float y = obj->axis[i].y;
		float z = obj->axis[i].z;

		obj->axis[i].y = y * cosT - z * sinT;
		obj->axis[i].z = y * sinT + z * cosT;
	}
}

void rotate_y(struct object_3D* obj, float theta){

	float cosT = cos(theta);
	float sinT = sin(theta);

	for(int i = 0; i < obj->points_size; i++){

		float x = obj->points[i].x;
		float z = obj->points[i].z;

		obj->points[i].x = x * cosT + z * sinT;
		obj->points[i].z = z * cosT - x * sinT;
	}

	for(int i = 1; i < 4; i++){

		float x = obj->axis[i].x;
		float z = obj->axis[i].z;

		obj->axis[i].x = x * cosT + z * sinT;
		obj->axis[i].z = z * cosT - x * sinT;
	}

}

void rotate_z(struct object_3D* obj, float theta){

	float cosT = cos(theta);
	float sinT = sin(theta);

	for(int i = 0; i < obj->points_size; i++){

		float x = obj->points[i].x;
		float y = obj->points[i].y;

		obj->points[i].x = x * cosT - y * sinT;
		obj->points[i].y = x * sinT + y * cosT;
	}

	for(int i = 1; i < 4; i++){

		float x = obj->axis[i].x;
		float y = obj->axis[i].y;

		obj->axis[i].x = x * cosT - y * sinT;
		obj->axis[i].y = x * sinT + y * cosT;
	}

}

void project_object(struct object_3D* obj, int resX, int resY){

	int distance = 150;

	for(int i = 0; i < obj->points_size; i++){

		struct point_3D* pt = &obj->points[i];

		float absPosX = pt->x + obj->x;
		float absPosY = pt->y + obj->y;
		float absPosZ = pt->z + obj->z + distance;

		if(absPosZ < 0.1){

			absPosZ = 0.1;
		}

		float zConversion = 250 / absPosZ;

		pt->screenX = absPosX * zConversion + (resX / 2);
		pt->screenY = absPosY * zConversion + (resY / 2);
	}

	if(obj->show_axis == 1){

		for(int i = 0; i < 4; i++){

			struct point_3D* pt = &obj->axis[i];

			float absPosX = pt->x + obj->x;
			float absPosY = pt->y + obj->y;
			float absPosZ = pt->z + obj->z + distance;

			if(absPosZ < 0.1){

				absPosZ = 0.1;
			}

			float zConversion = 250 / absPosZ;

			pt->screenX = absPosX * zConversion + (resX / 2);
			pt->screenY = absPosY * zConversion + (resY / 2);
		}
	}
}

void render_object(SDL_Renderer* renderer, struct object_3D* obj){

	int half_size = obj->points_size / 2;

	for(int i = 0; i < half_size; i++){

		struct point_3D* pt = &obj->points[i];
		struct point_3D* nx = &obj->points[(i + 1) % half_size];
		struct point_3D* op = &obj->points[i + half_size];
		struct point_3D* op_nx = &obj->points[(i + 1) % half_size + half_size];

		SDL_RenderDrawLine(renderer, pt->screenX, pt->screenY, nx->screenX, nx->screenY);
		SDL_RenderDrawLine(renderer, pt->screenX, pt->screenY, op->screenX, op->screenY);
		SDL_RenderDrawLine(renderer, op->screenX, op->screenY, op_nx->screenX, op_nx->screenY);
	}

	if(obj->show_axis == 1){

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, obj->axis[0].screenX, obj->axis[0].screenY, obj->axis[1].screenX, obj->axis[1].screenY);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, obj->axis[0].screenX, obj->axis[0].screenY, obj->axis[2].screenX, obj->axis[2].screenY);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(renderer, obj->axis[0].screenX, obj->axis[0].screenY, obj->axis[3].screenX, obj->axis[3].screenY);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
}
