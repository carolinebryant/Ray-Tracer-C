/* 
CPSC 102 
Project 3
Name: Caroline Bryant
*/

#ifndef VECTOR_H
#define VECTOR_H

// point and vector type structre 
typedef struct {

        double x;
        double y;
        double z;
} PV_T;

PV_T normalize(PV_T vector);
double dot_product(PV_T v1, PV_T v2);

#endif
