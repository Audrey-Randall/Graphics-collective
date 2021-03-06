#include "shaders.h"

//Check for errors
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

//Print error and exit
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Read text file
 */
 char* ReadText(char *file)
 {
    int   n;
    char* buffer;
    //  Open file
    FILE* f = fopen(file,"rt");
    if (!f) Fatal("Cannot open text file %s\n",file);
    //  Seek to end to determine size, then rewind
    fseek(f,0,SEEK_END);
    n = ftell(f);
    rewind(f);
    //  Allocate memory for the whole file
    buffer = (char*)malloc(n+1);
    if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
    //  Snarf the file
    int read;
    if ((read = fread(buffer,1,n,f)) < 0) {
      //printf("buffer is: \n");
      perror("Error was");
      Fatal("Cannot read %d bytes for text file %s\n",n,file);
    }
    if(read < n) {
      int i;
      char* newBuf = (char*)malloc(read+1);
      for(i = 0; i < read; i++) {
        newBuf[i] = buffer[i];
        //printf("%c", newBuf[i]);
      }
      newBuf[read] = 0;
      return newBuf;
    }
    buffer[n] = 0;
    //  Close and return
    fclose(f);
    return buffer;
 }

 void PrintShaderLog(int obj,char* file)
 {
    int len=0;
    glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
    if (len>1)
    {
       int n=0;
       char* buffer = (char *)malloc(len);
       if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
       glGetShaderInfoLog(obj,len,&n,buffer);
       fprintf(stderr,"%s:\n%s\n",file,buffer);
       free(buffer);
    }
    glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
    if (!len) Fatal("Error compiling %s\n",file);
 }

 /*
  *  Print Program Log
  */
 void PrintProgramLog(int obj)
 {
    int len=0;
    glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
    if (len>1)
    {
       int n=0;
       char* buffer = (char *)malloc(len);
       if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
       glGetProgramInfoLog(obj,len,&n,buffer);
       fprintf(stderr,"%s\n",buffer);
    }
    glGetProgramiv(obj,GL_LINK_STATUS,&len);
    if (!len) Fatal("Error linking program\n");
 }

int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}


/*
 *  Reverse n bytes
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

/*
 *  Load texture from BMP file
 */
unsigned int LoadTexBMP(const char* file)
{
   unsigned int   texture;    // Texture name
   FILE*          f;          // File pointer
   unsigned short magic;      // Image magic
   unsigned int   dx,dy,size; // Image dimensions
   unsigned short nbp,bpp;    // Planes and bits per pixel
   unsigned char* image;      // Image data
   unsigned int   off;        // Image offset
   unsigned int   k;          // Counter
   int            max;        // Maximum texture dimensions

   //  Open file
   f = fopen(file,"rb");
   if (!f) Fatal("Cannot open file %s\n",file);
   //  Check image magic
   if (fread(&magic,2,1,f)!=1) Fatal("Cannot read magic from %s\n",file);
   if (magic!=0x4D42 && magic!=0x424D) Fatal("Image magic not BMP in %s\n",file);
   //  Read header
   if (fseek(f,8,SEEK_CUR) || fread(&off,4,1,f)!=1 ||
       fseek(f,4,SEEK_CUR) || fread(&dx,4,1,f)!=1 || fread(&dy,4,1,f)!=1 ||
       fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
     Fatal("Cannot read header from %s\n",file);
   //  Reverse bytes on big endian hardware (detected by backwards magic)
   if (magic==0x424D)
   {
      Reverse(&off,4);
      Reverse(&dx,4);
      Reverse(&dy,4);
      Reverse(&nbp,2);
      Reverse(&bpp,2);
      Reverse(&k,4);
   }
   //  Check image parameters
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
   if (dx<1 || dx>max) Fatal("%s image width %d out of range 1-%d\n",file,dx,max);
   if (dy<1 || dy>max) Fatal("%s image height %d out of range 1-%d\n",file,dy,max);
   if (nbp!=1)  Fatal("%s bit planes is not 1: %d\n",file,nbp);
   if (bpp!=24) Fatal("%s bits per pixel is not 24: %d\n",file,bpp);
   if (k!=0)    Fatal("%s compressed files not supported\n",file);
#ifndef GL_VERSION_2_0
   //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
   for (k=1;k<dx;k*=2);
   if (k!=dx) Fatal("%s image width not a power of two: %d\n",file,dx);
   for (k=1;k<dy;k*=2);
   if (k!=dy) Fatal("%s image height not a power of two: %d\n",file,dy);
#endif

   //  Allocate image memory
   size = 3*dx*dy;
   image = (unsigned char*) malloc(size);
   if (!image) Fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
   //  Seek to and read image
   if (fseek(f,off,SEEK_SET) || fread(image,size,1,f)!=1) Fatal("Error reading data from image %s\n",file);
   fclose(f);
   //  Reverse colors (BGR -> RGB)
   for (k=0;k<size;k+=3)
   {
      unsigned char temp = image[k];
      image[k]   = image[k+2];
      image[k+2] = temp;
   }

   //  Sanity check
   ErrCheck("LoadTexBMP");
   //  Generate 2D texture
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   //  Copy image
   glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
   if (glGetError()) Fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
   //  Scale linearly when image size doesn't match
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   //  Free image memory
   free(image);
   //  Return texture name
   return texture;
}

void drawPlane(float width, float height, int split) {
  float i; //x
  float j; //y
  float inc = 2/(float)split;
  glBegin(GL_QUADS);
  glNormal3f(0,0,1);
  for(i = -1; i < 1-inc; i+=inc) {
    for(j = -1; j < 1-inc; j+=inc) {
      /*if(i <= 1-inc*2) {
        printf("\n\n (i,j) = (%f,%f)\n", i, j);
        printf("\tTexCoord 1: (%f,%f)\n", (i+1)/2, (j+1)/2);
        printf("\tTexCoord 2: (%f,%f)\n", (i+1)/2, (j+inc+1)/2);
        printf("\tTexCoord 3: (%f,%f)\n", (i+inc+1)/2, (j+inc+1)/2);
        printf("\tTexCoord 4: (%f,%f)\n", (i+inc+1)/2, (j+1)/2);
      }*/
      glTexCoord2f((i+1)/2, (j+1)/2);
      glVertex3d(i, j, 0);
      glTexCoord2f((i+1)/2, (j+inc+1)/2);
      glVertex3d(i, j+inc, 0);
      glTexCoord2f((i+inc+1)/2, (j+inc+1)/2);
      glVertex3d(i+inc, j+inc, 0);
      glTexCoord2f((i+inc+1)/2, (j+1)/2);
      glVertex3d(i+inc, j, 0);
    }
  }
  glEnd();
  /*glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(255, 0, 255);
  glNormal3f(0,0,1);
  for(i = -1; i < 1-inc; i+=inc) {
    for(j = -1; j < 1-inc; j+=inc) {
      glVertex3d(i, j, 0.0001);
      glVertex3d(i, j+inc, 0.0001);
      glVertex3d(i, j+inc, 0.0001);
      glVertex3d(i+inc, j+inc, 0.0001);
      glVertex3d(i+inc, j+inc, 0.0001);
      glVertex3d(i+inc, j, 0.0001);
      glVertex3d(i+inc, j, 0.0001);
      glVertex3d(i, j, 0.0001);
    }
  }
  glEnd();
  glEnable(GL_LIGHTING);*/
}

void drawWater(){
  glColor4f(0.5,0,0, 0.2);
  int i;
  int j;
  for(i = -5; i < 5; i++) {
    for(j = -5; j < 5; j++) {
      glPushMatrix();
      glRotated(90, 1,0,0);
      glTranslated(i*2, j*2, 0);
      drawPlane(1,1,10);
      glPopMatrix();
    }
  }
}

//Credit to http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
int renderToFrameBuf(){
  fbuf = 0;
	glGenFramebuffers(1, &fbuf);
	glBindFramebuffer(GL_FRAMEBUFFER, fbuf);

  glActiveTexture(GL_TEXTURE7);
  glGenTextures(1, &fbufTex);
	glBindTexture(GL_TEXTURE_2D, fbufTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbufTex, 0);

	// Set the list of draw buffers.
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return -1;
	else return 0;
}


void setUniforms(int shader, int frameInSec, unsigned int texture) {
    if(shader == shader_uw) {
      int texUnitLoc = glGetUniformLocation(shader_uw, "tex");
      if(texUnitLoc < 0) {
        printf("Failure in shader %d: uniform tex\n", shader_uw);
      } else {
        //set the uniform in shader_uw called tex to texture 0
        //glProgramUniform1i(shader_uw, texUnitLoc, tex_uw);
	glUniform1i(texUnitLoc, tex_uw);
      }
      int normalLoc = glGetUniformLocation(shader_uw, "normal_tex");
      if(normalLoc < 0) {
        printf("Failure in shader %d: uniform normal_tex\n", shader_uw);
      } else {
        //glProgramUniform1i(shader_uw, normalLoc, norm_uw);
	glUniform1i(normalLoc, norm_uw);
      }
      int frameLoc = glGetUniformLocation(shader_uw, "frame");
      if(frameLoc < 0) {
        //printf("Failure in shader crap: uniform frame. Main's frame = %f\n", frameInSec);
      } else {
        glUniform1i(frameLoc, frameInSec);
      }
    }
    else if(shader == shader_ws) {
      int texUnitLoc = glGetUniformLocation(shader_ws, "tex");
      if(texUnitLoc < 0) {
        printf("Failure in shader %d: uniform tex\n", shader_ws);
      } else {
        glUniform1i(texUnitLoc, tex_ws);
      }
      int normalLoc = glGetUniformLocation(shader_ws, "normal_tex");
      if(normalLoc < 0) {
        printf("Failure in shader %d: uniform normal_tex\n", shader_ws);
      } else {
        glUniform1i(normalLoc, norm_ws);
      }
      int frameLoc = glGetUniformLocation(shader_ws, "frame");
      if(frameLoc < 0) {
        //printf("Failure in shader crap: uniform frame. Main's frame = %f\n", frameInSec);
      } else {
        glUniform1i(frameLoc, frameInSec);
      }
    } else if (shader == shader_distort) {
      int texUnitLoc = glGetUniformLocation(shader_distort, "fb_tex");
      if(texUnitLoc < 0) {
        printf("Failure in shader %d: uniform fb_tex\n", shader_distort);
      } else {
        glUniform1i(texUnitLoc, fbufTex);
      }
      int timeLoc = glGetUniformLocation(shader_distort, "time");
      if(timeLoc < 0) {
        //printf("Failure in shader %d: uniform time\n", shader_distort);
      } else {
        glUniform1i(timeLoc, frameInSec);
      }
    } else if (shader == shader_texture) {
      int texUnitLoc = glGetUniformLocation(shader_texture, "tex");
      if(texUnitLoc < 0) {
        printf("Failure in shader_texture: uniform tex\n");
      } else {
        glUniform1i( texUnitLoc, texture);
      }
    }
}
