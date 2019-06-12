/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#ifndef BOX_H
#define BOX_H

#include "ray.h"

int intersect_box (OBJ_T object, RAY_T ray, PV_T *intersect_pt, PV_T *normal, double *t);
void swap (double *tx, double *ty);

#endif
