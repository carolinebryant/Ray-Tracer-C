/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "ray.h"

COLOR_T lighting (SCENE_T scene, PV_T intersect_pt, PV_T normal, COLOR_T obj_color, RAY_T ray, OBJ_T *closest_obj);
COLOR_T checkerboard (OBJ_T obj, PV_T closest_int_pt);
COLOR_T solid (OBJ_T obj, PV_T closest_int_pt);

#endif
