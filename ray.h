/* 
CPSC 102 
Project 3 
Name: Caroline Bryant
*/

#ifndef RAY_H
#define RAY_H

#include "vector.h"	

// ray type 
typedef struct {

        PV_T org;
        PV_T dir;
} RAY_T;

// sphere type 
typedef struct {

        PV_T ctr;
        double r;
} SPH_T;

// plane type 
typedef struct {

	PV_T normal;
	double d;
} PLANE_T;

typedef struct {

	PV_T ll;
	PV_T ur;
} BOX_T;

// color type 
typedef struct {

	double r; 
	double g;
	double b;
} COLOR_T;

typedef struct light {

	PV_T location;
	COLOR_T color;
	struct light *next;
} LIGHT_T;

typedef struct image {

	int height;
	int width;
	COLOR_T **img;
} IMG_T;

// object type
typedef struct object {

	double reflectivity;
	COLOR_T color;
	COLOR_T color2;
	union {
		SPH_T sphere;
		PLANE_T plane;
		BOX_T box;
	} geom;

	int (*intersect) (struct object, RAY_T, PV_T *, PV_T *, double *t);
	COLOR_T (*get_color) (struct object, PV_T closest_int_pt);
	struct object *next;
} OBJ_T;

typedef struct scene {

	double x_start;
	double y_start;
	double pixel_width;
	COLOR_T back_color;
	LIGHT_T *light;
	OBJ_T *objs;
} SCENE_T;

#endif
