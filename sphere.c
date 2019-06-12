/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#include <math.h>
#include "ray.h"
#include "sphere.h"

// gets intersection points for a sphere
int intersect_sph (OBJ_T object, RAY_T ray, PV_T *intersect_pt, PV_T *normal, double *t) {

	double a, b, c;

	// calculates a, b, and c to be used in quadratic formula
	a = 1;

	b = 2 * ((ray.dir.x * (ray.org.x - object.geom.sphere.ctr.x)) + 
		 (ray.dir.y * (ray.org.y - object.geom.sphere.ctr.y)) + 
		 (ray.dir.z * (ray.org.z - object.geom.sphere.ctr.z)));

	c = (((ray.org.x - object.geom.sphere.ctr.x) * 
	     (ray.org.x - object.geom.sphere.ctr.x)) + 
	     ((ray.org.y - object.geom.sphere.ctr.y) * 
	     (ray.org.y - object.geom.sphere.ctr.y)) + 
	     ((ray.org.z - object.geom.sphere.ctr.z) * 
             (ray.org.z - object.geom.sphere.ctr.z)) - 
             (object.geom.sphere.r * object.geom.sphere.r));

	double t0 = 0, t1 = 0;

	// checking if the discriminant is negative
	if (((b*b) - (4*a*c)) < 0) {
		return 0;
	}

	// computes quadratic formulas
		t0 = ((-b - sqrt((b*b) - (4*a*c))) / 2*a);
		t1 = ((-b + sqrt((b*b) - (4*a*c))) / 2*a);	

	// checking which t-value from quadratic formula to use to determine the intersection point 
	if (t0 < 0 && t1 < 0) {
		return 0;
	}

	else if (t1 > 0 && t0 < 0) {
		*t = t1;
	}

	else if (t1 < 0 && t0 > 0) {
		*t = t0;
	}

	else if (t1 < t0) {
		*t = t1;
	}

	else if (t0 < t1) {
		*t = t0;
	}

	intersect_pt->x = (ray.org.x + (ray.dir.x * (*t)));
	intersect_pt->y = (ray.org.y + (ray.dir.y * (*t)));
	intersect_pt->z = (ray.org.z + (ray.dir.z * (*t)));

	// computing the normal line
	normal->x = ((intersect_pt->x - object.geom.sphere.ctr.x) / object.geom.sphere.r);
	normal->y = ((intersect_pt->y - object.geom.sphere.ctr.y) / object.geom.sphere.r);
	normal->z = ((intersect_pt->z - object.geom.sphere.ctr.z) / object.geom.sphere.r);

	return 1;

}

