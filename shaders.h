#ifndef SHADERS_H
#define SHADERS_H
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

int shader_uw;
int shader_ws;
int shader_debug;
int shader_distort;
int shader_texture;
//underwater object textures
unsigned int tex_uw;
unsigned int norm_uw;
//water surface textures
unsigned int tex_ws;
unsigned int norm_ws;

//frame buffer
GLuint fbuf;
GLuint depthRenderBuffer;
GLenum DrawBuffers[1];
GLuint fbufTex;

int width;
int height;

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
void drawWater();
void setUniforms(int shader, int frameInSec, unsigned int texture);
int renderToFrameBuf();

#endif
