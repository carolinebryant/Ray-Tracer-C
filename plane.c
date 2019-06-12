/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#include <math.h>
#include "ray.h"
#include "plane.h"

// gets intersection points for a plane
int intersect_plane (OBJ_T object, RAY_T ray, PV_T *intersect_pt, PV_T *normal, double *t) {

	double dot_org, dot_dir, d;

	dot_org = dot_product(object.geom.plane.normal, ray.org);
	dot_dir = dot_product(object.geom.plane.normal, ray.dir);

	if (dot_dir == 0) { 
		return 0;
	}

	*t = ( (-(dot_org + object.geom.plane.d)) / dot_dir );

	if (*t < 0) {
		 return 0;
	}

	intersect_pt->x = (ray.org.x + (ray.dir.x * (*t)));
	intersect_pt->y = (ray.org.y + (ray.dir.y * (*t)));
	intersect_pt->z = (ray.org.z + (ray.dir.z * (*t)));

	*normal = object.geom.plane.normal;

	return 1;
}

