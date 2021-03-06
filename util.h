#ifndef UTIL_H
#define UTIL_H

//#define USEGLEW

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifdef USEGLEW
#define GLEW_STATIC
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

typedef struct {float x,y,z;} Point;

//#include "textfile.h"
//Shader thingies
/*GLhandleARB*/int v,f,f2,p;

//Textures
unsigned int checker;
unsigned int waterNormals;
unsigned int sky[2];

//WASD
int left;
int right;
int up;
int down;
//Mouse coordinates
int lastX;
int lastY;
int rotateCamera; //boolean set when right click held
double sensitivity; //determines translation of pixels-traversed-by-pointer to degrees-rotated-by-camera

//Lighting
double ltAng; //angle
double ltY;
int ltMove; //boolean
double ltPos[4];
int ltParam; //which property of light am I changing with j/l? 0 = emission, 1 = ambient, 2 = diffuse, 3 = specular
double emission;
double ambient;
double diffuse;
double specular;
double rgb;

//Materials
double matSpec;
double matEmit;
double shinyness;

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
int drawLight;
Point Lp;        // Light position in local coordinate system from ex 35
Point Nc, Ec;     // Far or near clipping plane in local coordinate system from ex 35

double frame;
int frameInSec;
#define SPINES 100
double spineAngles[SPINES*4];
#define LEN 8192  // Maximum length of text string
#define PI 3.1415926
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

void Print(const char* format , ...);
void sphericalToCartesian(double rho, double phi, double theta, double* x, double* y, double* z);
void cartesianToSpherical(double x, double y, double z, double *rho, int *phi, int *theta);
void cross(double v1[3], double v2[3], double res[3]);
void normalize(double* x, double* y, double* z);
void drawCone(double h, double r);
void drawCube();
void drawAxes();
void setSpineAngles(double * arr, int size);
void drawSeaUrchin();
void getLtPos();
void drawSphere();
void drawStairs(int n, float h, float w, float l);
void drawRoom();
void drawAWStairWall();
void setLight();
void Crout(double M[16],int I[4]);
void Transform(float x0,float y0,float z0,
               float Sx,float Sy,float Sz,
               float th,float ph);
Point Backsolve(double M[16],int I[4],double Bx,double By,double Bz,double Bw);
void init();

#endif
