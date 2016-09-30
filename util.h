#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

double w2h;
double cubeRotate;
int th;       // Azimuth of view angle
int ph;       // Elevation of view angle
double rh;    //rho of view angle
double z;     // Z variable
double w;     // W variable
double dim;   // Dimension of orthogonal box
double Ex;
double Ey;
double Ez;
int shouldMove;
double lookVec[3]; //Vector with length 3 currently in direction I'm looking FROM ME NOT ORIGIN. NOT A POINT.
double rightVec[3]; //Vector that points to my right at all times, currently unit.
double fov; //field of view
int isPersp; //perspective or orthogonal projection
int spineAnglesSet;
#define SPINES 100
double spineAngles[SPINES*4];
#define LEN 8192  // Maximum length of text string
#define PI 3.1415926
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

void Print(const char* format , ...);
void sphericalToCartesian(double rho, double phi, double theta, double* x, double* y, double* z);
void cartesianToSpherical(double x, double y, double z, double *rho, int *phi, int *theta);
void drawCone(double h, double r);
void drawCube();
void drawAxes();
void setSpineAngles(double * arr, int size);
void drawSeaUrchin();
#endif
