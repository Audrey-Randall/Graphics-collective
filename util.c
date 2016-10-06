#include "util.h"

char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

void init() {
	frame = 0.0;
	cubeRotate = 0;
	th=0;       // Azimuth of view angle
	ph=0;       // Elevation of view angle
	z=0;     // Z variable
	w=1;     // W variable
	dim=2;   // Dimension of orthogonal box
	shouldMove = 0;
	spineAnglesSet = 0;
	fov = 53;
	isPersp=0;
	rh = 2;
	lookVec[0] = 0;
	lookVec[1] = 0;
	lookVec[2] = -1;
	rightVec[0] = 1;
	rightVec[1] = 0;
	rightVec[2] = 0;
	Ex = 0;
	Ey = 0;
	Ez = 2;
	sensitivity = 0.3;
	ltAng = 0;
	ltY = 0;
	ltMove = 1;
	getLtPos();
	emission = 0.0;
	ambient = 0.3;
	diffuse = 1.0;
	specular = 1.0;
	rgb = 1.0;
	ltParam = 0;
	shinyness = 16;
	matEmit = emission;
	matSpec = specular;
}

void getLtPos(){
	ltPos[0] = 2*Cos(ltAng);
	ltPos[1] = ltY;
	ltPos[2] = 2*Sin(ltAng);
	ltPos[3] = 1.0;
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

void cartesianToSpherical(double x, double y, double z, double *rho, int *phi, int *theta) {
	*rho = sqrt(x*x+y*y+z*z);
	*theta = acosf(z/ *rho)*180/3.1415927;
	*phi = atanf(y/x)*180/3.1415927;
}

void cross(double v1[3], double v2[3], double res[3]) {
	res[0] = v1[1]*v2[2]-v2[1]*v1[2];
	res[1] = -1*(v1[0]*v2[2] - v2[0]*v1[2]);
	res[2] = v1[0]*v2[1] - v2[0]*v1[1];
}

void drawCone(double h, double r) {
  glBegin(GL_TRIANGLES);
  int i;
  for(i = 0; i <360; i+=15) {
		double v1[3] = {-r*Sin(i), -r*Cos(i), h};
		double v2[3] = {r*Sin(i+15)-r*Sin(i), r*Cos(i+15)-r*Cos(i), 0};
		double normal[3];
		cross(v1, v2, normal);
		//printf("Normal is (%f,%f,%f)\n", normal[0], normal[1], normal[2]);
		glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3d(0,0,h);
    glVertex3d(r*Sin(i),r*Cos(i), 0);
		glVertex3d(r*Sin(i+15), r*Cos(i+15), 0);
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

    glNormal3f(0,0,-1);
    glColor3f(1,0,0); //red
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(0,-1,0);
    glColor3f(0,1,0); //green
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(0,0,1);
    glColor3f(0,0,1); //blue
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);

    glNormal3f(1,0,0);
    glColor3f(1,0,1); //magenta
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f,-1.0f,-1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);

    glNormal3f(0,1,0);
    glColor3f(0,1,1); //cyan
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f, 1.0f);
    glVertex3d(1.0f, 1.0f,-1.0f);
    glVertex3d(-1.0f, 1.0f,-1.0f);

    glEnd();
    //glFlush();
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

void drawSphere(){
	//set r and h to draw cone for sphere top and bottom
	/*glBegin(GL_TRIANGLES);
  int i;
  for(i = 0; i <360; i+=15) {
		double v1[3] = {-r*Sin(i), -r*Cos(i), h};
		double v2[3] = {r*Sin(i+15)-r*Sin(i), r*Cos(i+15)-r*Cos(i), 0};
		double normal[3];
		cross(v1, v2, normal);
		//printf("Normal is (%f,%f,%f)\n", normal[0], normal[1], normal[2]);
		glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3d(0,0,h);
    glVertex3d(r*Sin(i),r*Cos(i), 0);
		glVertex3d(r*Sin(i+15), r*Cos(i+15), 0);
  }
  glEnd();*/
	glBegin(GL_QUADS);
	printf("\n\n\n");
	int phi;
	int theta;
	int blah = 0;
	for(phi = -180; phi < 180; phi+=15){
		for(theta = 0; theta < 360; theta+=15){
			double x;
			double y;
			double z;
			sphericalToCartesian(1, phi, theta, &x, &y, &z);
			glNormal3f(x,y,z);
			glVertex3d(x,y,z);
			if(blah == 0) printf("(%f,%f,%f)\n",x,y,z);
			sphericalToCartesian(1, phi+15, theta, &x, &y, &z);
			glNormal3f(x,y,z);
			glVertex3d(x,y,z);
			if(blah == 0) printf("(%f,%f,%f)\n",x,y,z);
			sphericalToCartesian(1, phi+15, theta+15, &x, &y, &z);
			glNormal3f(x,y,z);
			glVertex3d(x,y,z);
			if(blah == 0) printf("(%f,%f,%f)\n",x,y,z);
			sphericalToCartesian(1, phi, theta+15, &x, &y, &z);
			glNormal3f(x,y,z);
			glVertex3d(x,y,z);
			if(blah == 0) printf("(%f,%f,%f)\n",x,y,z);
			blah = 1;
		}
	}
	glEnd();
}

void setLight(){
	//Lighting variables, taken from example 26
  float Emission[]  = {emission, emission, emission,1.0};
  float Ambient[]   = {ambient, ambient, ambient,1.0};
  float Diffuse[]   = {diffuse, diffuse, diffuse,1.0};
  float Specular[]  = {specular, specular, specular,1.0};
	float Position[] = {2*Cos(ltAng), ltY, 2*Sin(ltAng), 1.0};
  float Shinyness[] = {shinyness};
	float RGBA[4] = {1,1,1,1};

	float MatSpec[] = {matSpec, matSpec, matSpec, 1.0};
	float MatEmit[] = {matEmit, matEmit, matEmit, 1.0};

	//  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION, Position);
  //  Set materials
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,MatEmit);
}
