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

//these are from Prof. Schreuder's example 27
char* ReadText(char *file);
int CreateShader(GLenum type,char* file);
int CreateShaderProg(char* VertFile, char* FragFile);

#endif
