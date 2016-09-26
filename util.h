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
double z;     // Z variable
double w;     // W variable
double dim;   // Dimension of orthogonal box
int obj; //loads my elf dude that I made in Sculptris. "Object display list."
int shouldMove;
int spineAnglesSet;
#define SPINES 100
double spineAngles[SPINES*4];
#define LEN 8192  // Maximum length of text string
#define PI 3.1415926
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

void Print(const char* format , ...);
void sphericalToCartesian(double rho, double phi, double theta, double* x, double* y, double* z);
void drawCone(double h, double r);
void drawCube();
void drawAxes();
void setSpineAngles(double * arr, int size);
void drawSeaUrchin();
#endif
