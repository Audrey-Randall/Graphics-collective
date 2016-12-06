

#include "util.h"

char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

void init() {
	frame = 0.0;
	frameInSec = 0;
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
	diffuse = 0.1;
	specular = 0.8;
	rgb = 1.0;
	ltParam = 0;
	shinyness = 16;
	matEmit = emission;
	matSpec = specular;
	drawLight = 1;
}

void getLtPos(){
	//2*Cos(ltAng), ltY, 2*Sin(ltAng), 1.0
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

void normalize(double* x, double* y, double* z) {
	double len=sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
	*x = *x/len;
	*y = *y/len;
	*z = *z/len;
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

//Same as drawCube but with texture coordinates
//i is which step, h = height of step,
drawStep(){
    glBegin(GL_TRIANGLES);
    glColor3f(0.38,0.355,0.30); //yellow
    glNormal3f(-1,0,0);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,1);
    glVertex3d(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,1);
    glVertex3d(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,0);
    glVertex3d(-1.0f, 1.0f,-1.0f);

    glNormal3f(0,0,-1);
    glColor3f(0.225,0.2,0.15);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f,-1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f, 1.0f,-1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f,-1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f,-1.0f,-1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(0,-1,0);
    glColor3f(0.613,0.5277,0.3611); //green
		glTexCoord2f(1,1);
    glVertex3d(1.0f,-1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f,-1.0f, 1.0f);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f,-1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f,-1.0f);

    glNormal3f(0,0,1);
    glColor3f(0.38,0.355,0.30);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f,-1.0f, 1.0f);

    glNormal3f(1,0,0);
    glColor3f(0.225,0.2,0.15);
		glTexCoord2f(0,0);
    glVertex3d(1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,1);
    glVertex3d(1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f, 1.0f,-1.0f);

    glNormal3f(0,1,0);
    glColor3f(0.613,0.5277,0.3611);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f, 1.0f,-1.0f);
		glTexCoord2f(0,1);
    glVertex3d(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,1);
    glVertex3d(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,0);
    glVertex3d(1.0f, 1.0f,-1.0f);
		glTexCoord2f(0,0);
    glVertex3d(-1.0f, 1.0f,-1.0f);

    glEnd();
    //glFlush();
}

//n is number of stairs, h is height in world coords
void drawStairs(int n, float h, float w, float l) {
	int i;
	for(i = 1; i < n; i++) {
		glPushMatrix();
		glTranslated((h/n*i)*4*l, h/n*i, 0);
		glScaled(l/n,h/n*i, w);
		drawStep();
		glPopMatrix();
	}
}

void drawUWStairWall(){
	float i, j;
	glPushMatrix();
	glScaled(0.5,0.5,0.1);
	for(i = -2.5; i < 1; i++){
		for(j = -2.5; j < 0; j++){
			glPushMatrix();
			glTranslated(i*2, j*2, 0);
			drawStep();
			glPopMatrix();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5,0,0);
	//Upper stairs
	glPushMatrix();
	glTranslated(1.5, -1.5,3.55);
	glRotated(90,0,1,0);
	glScaled(1,1.5,1);
	drawStairs(10,0.5,1, 2);
	glPopMatrix();

	//Lower stairs
	glPushMatrix();
	glTranslated(-3.3, -3, 4.33);
	//glRotated(90,0,1,0);
	glScaled(1,1.5,1);
	drawStairs(10,0.5,1, 2);
	glPopMatrix();

	//Corner cube for stair
	glPushMatrix();
	glTranslated(1.5, -2.5, 4.3);
	//glScaled(1,0.75,1);
	drawStep();
	glPopMatrix();

	//Side cube under high stair
	glPushMatrix();
	glTranslated(1.5, -2.5, 1.75);
	glScaled(1,1,1.75);
	drawStep();
	glPopMatrix();

	glPopMatrix();

	//Floor
	glPushMatrix();
	glTranslated(0, -3, 3);
	glRotated(90,1,0,0);
	glPushMatrix();
	glScaled(0.5,0.5,0.1);
	for(i = -2.5; i < 2.5; i++){
		for(j = -2.5; j < 2.5; j++){
			glPushMatrix();
			glTranslated(i*2, j*2, 0);
			drawStep();
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPopMatrix();

	//Side walls
	int k;
	for(k = -3; k <=3; k+=6){
		glPushMatrix();
		glTranslated(k, 0, 2);
		glRotated(90,0,1,0);
		glPushMatrix();
		glScaled(0.5,0.5,0.1);
		for(i = -2.5; i < 2.5; i++){
			for(j = -2.5; j < 0; j++){
				glPushMatrix();
				glTranslated(i*2, j*2, 0);
				drawStep();
				glPopMatrix();
			}
		}
		glPopMatrix();
	  glPopMatrix();
	}

}

void drawAWStairWall(){

}

void drawRoom(){
	drawUWStairWall();
}

void setLight(){
	//printf("Light is at (%f, %f, %f)\n", 2*Cos(ltAng), ltY, 2*Sin(ltAng));
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

/*
 *  All credit goes to Professor Schreuder
 *  Perform Crout LU decomposition of M in place
 *     M  4x4 matrix
 *     I  Pivot index
 *  Calls Fatal if the matrix is singular
 */
#define M(i,j) (M[4*j+i])
void Crout(double M[16],int I[4])
{
   int i,j,k;  // Counters

   //  Initialize index
   for (j=0;j<4;j++)
      I[j] = j;

   //  Pivot matrix to maximize diagonal
   for (j=0;j<3;j++)
   {
      //  Find largest absolute value in this column (J)
      int J=j;
      for (k=j+1;k<4;k++)
         if (fabs(M(k,j)) > fabs(M(J,j))) J = k;
      //  Swap rows if required
      if (j!=J)
      {
         k=I[j]; I[j]=I[J]; I[J]=k;
         for (k=0;k<4;k++)
         {
            double t=M(j,k); M(j,k)=M(J,k); M(J,k)=t;
         }
      }
   }

   //  Perform Crout LU decomposition
   for (j=0;j<4;j++)
   {
      //  Upper triangular matrix
      for (i=j;i<4;i++)
         for (k=0;k<j;k++)
            M(j,i) -= M(k,i)*M(j,k);
      if (fabs(M(j,j))<1e-10) Fatal("Singular transformation matrix\n");
      //  Lower triangular matrix
      for (i=j+1;i<4;i++)
      {
         for (k=0;k<j;k++)
            M(i,j) -= M(k,j)*M(i,k);
         M(i,j) /= M(j,j);
      }
   }
}

/*
 *  All credit goes to Professor Schreuder
 *  Backsolve LU decomposition (aka back substitution, given M solve for b)
 *     M  4x4 matrix
 *     I  Pivot index
 *     Bx,By,Bz,Bw is RHS
 *     Returns renormalized Point
 */
Point Backsolve(double M[16],int I[4],double Bx,double By,double Bz,double Bw)
{
   int    i,j;                  //  Counters
   double x[4];                 //  Solution vector
   Point  X;                    //  Solution Point
   double b[4] = {Bx,By,Bz,Bw}; //  RHS

   //  Backsolve
   for (i=0;i<4;i++)
   {
      x[i] = b[I[i]];
      for (j=0;j<i;j++)
         x[i] -= M(i,j)*x[j];
   }
   for (i=3;i>=0;i--)
   {
      for (j=i+1;j<4;j++)
         x[i] -= M(i,j)*x[j];
      x[i] /= M(i,i);
   }

   //  Renormalize
   if (fabs(x[3])<1e-10) Fatal("Light position W is zero\n");
   X.x = x[0]/x[3];
   X.y = x[1]/x[3];
   X.z = x[2]/x[3];
   return X;
}

/*
 *  Credit for this code goes to Professor Schreuder
 *  Set up transform
 *     Push and apply transformation
 *     Calculate light position in local coordinate system (Lp)
 *     Calculate clipping plane in local coordinate system (Ec,Nc)
 *  Global variables set: Lp,Nc,Ec
 *  Killer fact 1:  You MUST call this to set transforms for objects
 *  Killer fact 2:  You MUST call glPopMatrix to undo the transform
 */
void Transform(float x0,float y0,float z0,
               float Sx,float Sy,float Sz,
               float th,float ph)
{
   double M[16]; // Transformation matrix
   int    I[4];  // Pivot
   double Z;     // Location of clip plane

   //  Save current matrix
   glPushMatrix();

   //  Build transformation matrix and copy into M
   glPushMatrix();
   glLoadIdentity();
   glTranslated(x0,y0,z0);
	 //these are shadowed, parameters, not the globals
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScaled(Sx,Sy,Sz);
	 //put modelview matrix in M
   glGetDoublev(GL_MODELVIEW_MATRIX,M);
   glPopMatrix();

   //  Apply M to existing transformations
   glMultMatrixd(M);

   /*
    *  Determine light position in this coordinate system
    */
   Crout(M,I);
   Lp = Backsolve(M,I,ltPos[0],ltPos[1],ltPos[2],ltPos[3]);

   /*
    *  Determine clipping plane E & N
    *  Use the entire MODELVIEW matrix here
    */
   glGetDoublev(GL_MODELVIEW_MATRIX,M);
   //  Normal is down the Z axis (0,0,1) since +/- doesn't matter here
   //  The normal matrix is the inverse of the transpose of M
   Nc.x = M(2,0);
   Nc.y = M(2,1);
   Nc.z = M(2,2);
   //  Far  clipping plane for Z-fail should be just less than 8*dim
   //  Near clipping plane for Z-pass should be just more than dim/8
   Crout(M,I);
   //Z = (mode==5) ? -7.9*dim : -0.13*dim; //Not sure what mode 5 was in ex. 35
	 Z = -0.13*dim;
   Ec = Backsolve(M,I,0,0,Z,1);
}
