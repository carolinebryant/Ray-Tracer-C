/* 
CPSC 102 
Project 3 
Name: Caroline Bryant
*/

#include <math.h>
#include "vector.h"
#include "ray.h"

// normalize vector function, divide each vector by its length
PV_T normalize(PV_T vector) {

	PV_T ret_v;
	double len;
	len = sqrt((vector.x*vector.x) + (vector.y*vector.y) + (vector.z*vector.z));

	ret_v.x = vector.x/len;
	ret_v.y = vector.y/len;
	ret_v.z = vector.z/len;

	return ret_v;
}

// computes dot product, multiply each component of the normal vector by the light vector
double dot_product(PV_T vector1, PV_T vector2) {

	double result;
	result = ((vector1.x*vector2.x) + (vector1.y*vector2.y) + (vector1.z*vector2.z));

	return result;
}


