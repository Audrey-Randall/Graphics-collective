#include "util.h"

char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

void init() {
	cubeRotate = 0;
	th=0;       // Azimuth of view angle
	ph=0;       // Elevation of view angle
	z=0;     // Z variable
	w=1;     // W variable
	dim=2;   // Dimension of orthogonal box
	shouldMove = 1;
	spineAnglesSet = 0;
}

void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void sphericalToCartesian(double rho, double phi, double theta, double* x, double* y, double* z) {
  *x = rho*Sin(theta)*Cos(phi);
  *y = rho*Sin(theta)*Cos(phi);
  *z = rho*Cos(theta);
}

void drawCone(double h, double r) {
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,1);
  glVertex3d(0,0,h);
  int i;
  for(i = 0; i <=360; i+=15) {
    glNormal3f(Sin(i), Cos(i), 0);
    glVertex3d(r*Sin(i),r*Cos(i), 0);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,-1);
  glVertex3d(0,0,0);
  for(i = 0; i <=360; i+=15) {
    glVertex3d(r*Sin(i),r*Cos(i), 0);
  }
  glEnd();
}

void drawCube() {
    glBegin(GL_TRIANGLES);
    glColor3f(1,1,0); //yellow
    glNormal3f(-1,0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);

    glNormal3f(0,-1,0);
    glColor3f(1,0,0); //red
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(0,0,-1);
    glColor3f(0,1,0); //green
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(1,0,0);
    glColor3f(0,0,1); //blue
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);

    glNormal3f(0,1,0);
    glColor3f(1,0,1); //magenta
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);

    glNormal3f(0,0,1);
    glColor3f(0,1,1); //cyan correct
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);

    glEnd();
    glFlush();
}

void drawAxes(){
  //Draw axes
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(1,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,1,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,1);
  glEnd();
}

void setSpineAngles(double * arr, int size) {
  srand(time(NULL));
  int i;
  for(i = 0; i < size; i++) {
    if(i%4) arr[i] = (double)(rand()%360-180);
    else arr[i] = (double)((rand()%100)-50);
  }
  spineAnglesSet = 1;
}

void drawSeaUrchin(){
  int i;
  if(!spineAnglesSet) setSpineAngles(spineAngles, SPINES*4);
  for(i = 0; i < SPINES; i++) {
    glPushMatrix();
    double spineAngle[4] = {spineAngles[i], spineAngles[i+1], spineAngles[i+2], spineAngles[i+3]};
    glRotated(spineAngle[0], spineAngle[1], spineAngle[2], spineAngle[3]);
    drawCone(1, 0.05);
    glPopMatrix();
  }
  //glutSolidSphere(0.7, 15, 15);
}
