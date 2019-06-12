/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#include <stdio.h>
#include <math.h>
#include "ray.h"
#include "vector.h"
#include "light.h"

COLOR_T lighting (SCENE_T scene, PV_T intersect_pt, PV_T normal, COLOR_T obj_color, RAY_T ray, OBJ_T *closest_obj) {

	PV_T L, dist_vect;
	double result, prod;
	PV_T dummy_norm;
	double dummy_t;
	double dist_light = 0;
	double c1 = .002, c2 = .02, c3 = .2;
	double attenuation = 0;

	// ambient light
	COLOR_T new_color;
	new_color.r = (0.1 * obj_color.r);
	new_color.g = (0.1 * obj_color.g);
	new_color.b = (0.1 * obj_color.b);

	int i;
	LIGHT_T *curr;
	
	for (curr = scene.light; curr != NULL; curr = curr->next) {
		
		if ( !shadow (scene, closest_obj, intersect_pt, &dummy_norm, &dummy_t, i, *curr) ) {

			// attenuation
			dist_vect.x = (intersect_pt.x - curr->location.x); 
			dist_vect.y = (intersect_pt.y - curr->location.y); 
			dist_vect.z = (intersect_pt.z - curr->location.z);

			dist_light = sqrt((dist_vect.x * dist_vect.x) + (dist_vect.y * dist_vect.y) + (dist_vect.z * dist_vect.z));

			attenuation = ( 1 / ((c1 * (dist_light * dist_light)) + (c2 * dist_light) + c3) ); 

			if (attenuation > 1) {

				attenuation = 1;
			}

			// diffuse lighting
			L.x = curr->location.x - intersect_pt.x;
			L.y = curr->location.y - intersect_pt.y;
			L.z = curr->location.z - intersect_pt.z;

			// normalize vector L
			L = normalize(L);

			// result holds dot product of the normal line and the light vector
			result = dot_product(normal, L);

			// if dot product is greater than zero, add ambient and diffuse lighting together
			if (result > 0) {

				new_color.r += ((result * obj_color.r * attenuation) * (1 - closest_obj->reflectivity)); 
				new_color.g += ((result * obj_color.g * attenuation) * (1 - closest_obj->reflectivity)); 
				new_color.b += ((result * obj_color.b * attenuation) * (1 - closest_obj->reflectivity)); 

				// structure to hold vector R
				PV_T R;  
	
				// specular lighting
				R.x = (L.x - (2 * (result) * normal.x));
				R.y = (L.y - (2 * (result) * normal.y));
				R.z = (L.z - (2 * (result) * normal.z));

				// normalize vector R
				R = normalize(R);

				// prod holds dot product of vector R and the ray direction 
				prod = dot_product(R, ray.dir);

				// if dot product is greater than zero, adds specular lighting  
				if (prod > 0) {

					new_color.r += pow(prod, 100);
 					new_color.g += pow(prod, 100);
					new_color.b += pow(prod, 100);	

				} // if (prod > 0)

			} // if (result > 0)

		} // if (!shadow) 

	} // end of for loop

	return new_color;
}  

// pass *closest_obj
int shadow (SCENE_T scene, OBJ_T *closest_obj, PV_T intersect_pt, PV_T *normal, double *t, int i, LIGHT_T light) {

	RAY_T shadow_ray;
	PV_T dummyNorm;

	OBJ_T *curr;

		// finding the direction of the shadow ray
		shadow_ray.dir.x = (light.location.x - intersect_pt.x);
		shadow_ray.dir.y = (light.location.y - intersect_pt.y);
		shadow_ray.dir.z = (light.location.z - intersect_pt.z);
		shadow_ray.org = intersect_pt;

		shadow_ray.dir = normalize(shadow_ray.dir);


		// checks if object is within a shadow
		for (curr = scene.objs; curr != NULL; curr = curr->next) {

			if (curr != closest_obj) {
							
				if (curr->intersect (*curr, shadow_ray, &dummyNorm, normal, t) ) {

					return 1;
				}
			}
		} // end of max objects

	return 0;
}

// function for solid colored objects
COLOR_T solid (OBJ_T obj, PV_T closest_int_pt) {

	return obj.color;
} 

// function for checkerboard pattern
COLOR_T checkerboard (OBJ_T obj, PV_T closest_int_pt) {


	if ( ( (int)floor(closest_int_pt.x) + (int)floor(closest_int_pt.y) + (int)floor(closest_int_pt.z) ) & 1) {

		return obj.color;
	}

	else {

		return obj.color2;
	}		
	

}

