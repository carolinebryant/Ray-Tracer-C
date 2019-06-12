/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#include <math.h>
#include "ray.h"
#include "box.h"

int intersect_box (OBJ_T objs, RAY_T ray, PV_T *intersect_pt, PV_T *normal, double *t) {

	// box normals  
	static PV_T normalArr[6];
	normalArr[0].x = -1;
	normalArr[0].y = 0; 
	normalArr[0].z = 0; // left side

	normalArr[1].x = 1;
	normalArr[1].y = 0;
	normalArr[1].z = 0; // right side

	normalArr[2].x = 0;
	normalArr[2].y = -1;
	normalArr[2].z = 0; // top side

	normalArr[3].x = 0; 
	normalArr[3].y = 1; 
	normalArr[3].z = 0; // bottom side

	normalArr[4].x = 0;
	normalArr[4].y = 0;
	normalArr[4].z = -1; // front side

	normalArr[5].x = 0; 
	normalArr[5].y = 0; 
	normalArr[5].z = 1; // back side

	// arrays for intersection points
	double boxMin[3]; 
	double boxMax[3];
	double rayOrg[3];
	double rayDir[3];

	boxMin[0] = objs.geom.box.ll.x;
	boxMin[1] = objs.geom.box.ll.y;
	boxMin[2] = objs.geom.box.ll.z;

	boxMax[0] = objs.geom.box.ur.x;
	boxMax[1] = objs.geom.box.ur.y;
	boxMax[2] = objs.geom.box.ur.z;

	rayOrg[0] = ray.org.x;
	rayOrg[1] = ray.org.y;
	rayOrg[2] = ray.org.z;

	rayDir[0] = ray.dir.x;
	rayDir[1] = ray.dir.y;
	rayDir[2] = ray.dir.z;

	double tnear = -1000;
	double tfar = 1000;
	double t1, t2;
	int ifswap = 0;

	int index = 0;
	int i;

	for (i = 0; i < 3; i++) {

		if (rayDir[i] == 0) {

			if (rayOrg[i] < boxMin[i] || rayOrg[i] > boxMax[i]) {
			
				return 0;
			}
		}

		t1 = (boxMin[i] - rayOrg[i]) / rayDir[i];
		t2 = (boxMax[i] - rayOrg[i]) / rayDir[i];
		ifswap = 0;

		if (t1 > t2) {

			swap(&t1, &t2);
			ifswap = 1;
		}

		if (t1 > tnear) {

			tnear = t1;
			index = 2 * i + ifswap;
		}

		if (t2 < tfar) {

			tfar = t2;
		}

		if (tnear > tfar) {

			return 0;
		}

		if (tfar < 0) {

			return 0;
		}			

	} // end of for loop

	// tnear is good
	*t = tnear;

	normal->x = normalArr[index].x;
	normal->y = normalArr[index].y;
	normal->z = normalArr[index].z;


	intersect_pt->x = rayOrg[0] + rayDir[0] * (*t);
	intersect_pt->y = rayOrg[1] + rayDir[1] * (*t);
	intersect_pt->z = rayOrg[2] + rayDir[2] * (*t);

	return 1;
}

void swap (double *tx, double *ty) {

	double temp;

	temp = *tx;
	*tx = *ty;
	*ty = temp;
}	
	
