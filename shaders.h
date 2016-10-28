#ifndef SHADERS_H
#define SHADERS_H
#define USEGLEW

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int shader1;

//from Prof. Schreuder's example 19
void ErrCheck(const char* where);
void Fatal(const char* format , ...);
static void Reverse(void* x,const int n);
unsigned int LoadTexBMP(const char* file);
//these are from Prof. Schreuder's example 27
char* ReadText(char *file);
void PrintProgramLog(int obj);
void PrintShaderLog(int obj,char* file);
int CreateShader(GLenum type,char* file);
int CreateShaderProg(char* VertFile, char* FragFile);
void drawPlane(float width, float height, int split);

#endif
