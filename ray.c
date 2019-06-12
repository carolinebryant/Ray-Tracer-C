/* 
CPSC 102 
Project 3 
Name: Caroline Bryant
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ray.h"
#include "vector.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "box.h"

static COLOR_T trace (SCENE_T scene, RAY_T ray, PV_T intersect_pt, PV_T normal, int level);
static void init (SCENE_T *scene, IMG_T *img, char filename[]); 

int main (int argc, char *argv[]) {

	RAY_T ray;
	PV_T intersect_pt;
	PV_T normal;

	COLOR_T obj_color;
	obj_color.r = 0;
	obj_color.g = 0;
	obj_color.b = 0;

	int x = 0, y = 0;

	unsigned char ppmr, ppmg, ppmb;
	int level = 0;
	
	SCENE_T scene;
	IMG_T img;

	// file I/O
	char filename[30];
	char outfile[30];
	FILE *fileout;

	if (argc == 1) {

		sprintf(filename, "scene.txt");
		sprintf(outfile, "img.ppm");
	}

	if (argc == 2) {

		sscanf(argv[1], "%s", filename);
		sprintf(outfile, "img.ppm");
	}

	if (argc == 3) {

		sscanf(argv[1], "%s", filename);
		sscanf(argv[2], "%s", outfile);
	}

	fileout = fopen(outfile, "w");

	// initalize scene values from file
	init(&scene, &img, filename);

	scene.x_start = ((-(float)img.width / (float)img.height)/ 2);
	scene.y_start = 0.5;
	scene.pixel_width = img.height;

	// ppm header 
	fprintf(fileout, "P6\n %i %i 255\n", img.width, img.height);

	// for each pixel
	for (y = 0; y < img.height; y++) {
		for (x = 0; x < img.width; x++) {

			// compute ray origin  
			ray.org.x = 0;
			ray.org.y = 0;
			ray.org.z = 0;

			// ray direction 
			ray.dir.x = scene.x_start + ((double)x/ scene.pixel_width);
			ray.dir.y = scene.y_start - ((double)y/ scene.pixel_width);
			ray.dir.z = 1; 

			// normalize ray direction 
			ray.dir = normalize(ray.dir);

			obj_color = trace(scene, ray, intersect_pt, normal, level); 

			// caps r g b values at 1 
			if (obj_color.r > 1) {
				obj_color.r = 1;
			}

			if (obj_color.g > 1) {
				obj_color.g = 1;
			}

			if (obj_color.b > 1) {
				obj_color.b = 1;
			}

			img.img[y][x].r = obj_color.r;
			img.img[y][x].g = obj_color.g;
			img.img[y][x].b = obj_color.b;

			// converts double r g b values back to unsigned char to be printed
			ppmr = (unsigned char)(obj_color.r*255);
			ppmg = (unsigned char)(obj_color.g*255);
			ppmb = (unsigned char)(obj_color.b*255);
						
			fprintf(fileout, "%c%c%c", ppmr, ppmg, ppmb);
					
		} // end of inner for loop

	} // end of outer for loop

	// free memory
	OBJ_T *currfree;
	currfree = scene.objs;

	while (currfree != NULL) {

		scene.objs = scene.objs->next;
		free(currfree);
		currfree = scene.objs;
	}

	return 0;
}

static COLOR_T trace (SCENE_T scene, RAY_T ray, PV_T intersect_pt, PV_T normal, int level) {

	COLOR_T color = scene.back_color;
	COLOR_T obj_color;
	COLOR_T final_col = color;
	COLOR_T reflect_col; 
	COLOR_T black = {0, 0, 0};

	PV_T closest_int_pt;
	PV_T closest_normal;
	PV_T R; 
	RAY_T new_ray;

	double closest_t = 1000;
	double t;
	int i;	
	double dot;
	
	OBJ_T *closest_obj = NULL;
	OBJ_T *curr = NULL;
	
	// reflective ray
	dot = dot_product(normal, ray.dir);

	R.x = (ray.dir.x - 2 * (dot) * normal.x); 
	R.y = (ray.dir.y - 2 * (dot) * normal.y);
	R.z = (ray.dir.z - 2 * (dot) * normal.z);

	R = normalize(R);

	new_ray.org.x = intersect_pt.x; 
	new_ray.org.y = intersect_pt.y;
	new_ray.org.z = intersect_pt.z; 

	new_ray.dir.x = R.x; 
	new_ray.dir.y = R.y;
	new_ray.dir.z = R.z; 

	// cap reflectivity
	if (level > 8) {

		return black;
	}

	// loop to determine closest object, clostest intersection point and normal 
	for (curr = scene.objs; curr != NULL; curr = curr->next) {

		if (curr->intersect(*curr, ray, &intersect_pt, &normal, &t)) {

			if (t > 0.01) {

				if (t < closest_t) {

					closest_t = t;
					closest_obj = curr;
					closest_int_pt = intersect_pt;
					closest_normal = normal;
				}
			}

		}
	} // end of for loop

			// if an object was intersected, illuminates the object 
			if (closest_obj != NULL) {

				if (closest_obj->reflectivity > 0) {

					reflect_col = trace (scene, new_ray, intersect_pt, normal, level+1);
				}

				if (closest_obj->reflectivity < 1) {

					obj_color = closest_obj->get_color (*closest_obj, closest_int_pt);			

					color = lighting(scene, closest_int_pt, closest_normal, obj_color, ray, closest_obj);
				}
				
				final_col.r = ((reflect_col.r * closest_obj->reflectivity) + 
						(color.r * (1 - closest_obj->reflectivity)));

				final_col.g = ((reflect_col.g * closest_obj->reflectivity) + 
						(color.g * (1 - closest_obj->reflectivity)));

				final_col.b = ((reflect_col.b * closest_obj->reflectivity) + 
						(color.b * (1 - closest_obj->reflectivity)));
			}


	return final_col;
}

// initalizes objects within the array and their values
static void init (SCENE_T *scene, IMG_T *img, char filename[]) {

	double x, y, z;
	double r, g, b;

	OBJ_T *new_obj;
	LIGHT_T *light;

	FILE *fp;
	char line[100];
	char token[20];
	fp = fopen(filename, "r");

	// check for invalid file 
	if (fp == NULL) {

		fprintf(stderr, "file not found\n");
		exit(1);
	}

	// sets the end of the linked lists equal to null
	scene->objs = NULL;
	scene->light = NULL;
	
	while (fgets(line, 100, fp) != NULL) {

		token[0] = '\0';

		sscanf(line, "%s", token);


		if (strcmp(token, "width") == 0) {

			fscanf(fp, "%d", &img->width);
			
		} 

		if (strcmp(token, "height") == 0) {
			
			fscanf(fp, "%d", &img->height);

		} 

		if (strcmp(token, "background") == 0) {

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			scene->back_color.r = r;
			scene->back_color.g = g;			
			scene->back_color.b = b;

		} 

		if (strcmp(token, "sphere") == 0) {

			new_obj = (OBJ_T*)malloc(sizeof(OBJ_T));
			new_obj->next = NULL;

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			new_obj->geom.sphere.ctr.x = x; 
			new_obj->geom.sphere.ctr.y = y; 
			new_obj->geom.sphere.ctr.z = z; 

			fscanf(fp, "%lf", &r);
			new_obj->geom.sphere.r = r;

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			new_obj->color.r = r;
			new_obj->color.g = g;
			new_obj->color.b = b;

			fscanf(fp, "%lf", &r);
			new_obj->reflectivity = r;

			// set function pointers
			new_obj->intersect = intersect_sph;
			new_obj->get_color = solid;
			
			new_obj->next = scene->objs;
			scene->objs = new_obj;

		}

		if (strcmp(token, "plane") == 0) {

			new_obj = (OBJ_T*)malloc(sizeof(OBJ_T));
			new_obj->next = NULL;

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			new_obj->geom.plane.normal.x = x;
			new_obj->geom.plane.normal.y = y;
			new_obj->geom.plane.normal.z = z;

			fscanf(fp, "%lf", &r);
			new_obj->geom.plane.d = r;

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			new_obj->color.r = r;
			new_obj->color.g = g;
			new_obj->color.b = b;

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			new_obj->color2.r = r;
			new_obj->color2.g = g;
			new_obj->color2.b = b;

			fscanf(fp, "%lf", &r);
			new_obj->reflectivity = r;

			// set function pointers 
			new_obj->intersect = intersect_plane;
			new_obj->get_color = checkerboard;
		
			new_obj->next = scene->objs;
			scene->objs = new_obj;

		}

		if (strcmp(token, "box") == 0) {

			new_obj = (OBJ_T*)malloc(sizeof(OBJ_T));
			new_obj->next = NULL;

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			new_obj->geom.box.ll.x = x;
			new_obj->geom.box.ll.y = y;
			new_obj->geom.box.ll.z = z;

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			new_obj->geom.box.ur.x = x;
			new_obj->geom.box.ur.y = y;
			new_obj->geom.box.ur.z = z;

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			new_obj->color.r = r;
			new_obj->color.g = g;
			new_obj->color.b = b;

			fscanf(fp, "%lf", &r);
			new_obj->reflectivity = r;
			
			// set function pointers
			new_obj->intersect = intersect_box;
			new_obj->get_color = solid;

			new_obj->next = scene->objs;
			scene->objs = new_obj;

		}

		if (strcmp(token, "light") == 0) {

			light = (LIGHT_T*)malloc(sizeof(LIGHT_T));
			light->next = NULL;

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			light->location.x = x;
			light->location.y = y;
			light->location.z = z;

			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			light->color.r = r;
			light->color.g = g;
			light->color.b = b;			

			light->next = scene->light;
			scene->light = light;

		}

	}

	// initalizing 2d array for colors
	img->img = (COLOR_T**)malloc(img->height*sizeof(COLOR_T*));

	int i;
	for (i = 0; i < img->width; i++) {

		img->img[i] = (COLOR_T*)malloc(img->width*sizeof(COLOR_T));
	}
	
	fclose(fp);

} // end of init

