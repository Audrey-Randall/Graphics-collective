
#include "util.h"
/*
  Audrey Randall's homework 4
  Citations:
    example 26 from class
    http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
*/

float RGBA[4] = {1,1,1,1};

//Projection function.
void Project(double fov,double asp,double dim)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (isPersp)
	  //This sets up a perspective matrix, but doesn't actually reproject the scene. It defines the size of the truncated pyramid. Analogy: choose a lens for a camera.
      gluPerspective(fov,asp,dim/16,16*dim);
   else
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
   glMatrixMode(GL_MODELVIEW);
   //why is this necessary?
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Increase w by 0.1
   else if (ch == '+')
   {
      w += 0.1;
   }
   //  Decrease w by 0.1
   else if (ch == '-')
   {
      w -= 0.1;
   }
   else if(ch == '1') {
     shouldMove=!shouldMove;
   }
   else if (ch == 'f') {
	 if(fov < 85) fov+=5;
   }
   else if (ch == 'F') {
	 if(fov > 25) fov-=5;
   }
   else if (ch == 'g') {
	 fov=0;
   }
   else if (ch == 'p') {
	    isPersp = !isPersp;
   }
   else if(ch == 'w') {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     double step[3];
     double focLen = sqrt((lookVec[0])*(lookVec[0])+(lookVec[1])*(lookVec[1])+(lookVec[2])*(lookVec[2]));
     printf("focLen = %f\n", focLen);
     Ex += 0.2*lookVec[0]/focLen;
     Ey += 0.2*lookVec[1]/focLen;
     Ez += 0.2*lookVec[2]/focLen;
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   else if(ch == 's') {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     double focLen = sqrt((lookVec[0])*(lookVec[0])+(lookVec[1])*(lookVec[1])+(lookVec[2])*(lookVec[2]));
     printf("focLen is %f\n", focLen);
     Ex -= 0.2*lookVec[0]/focLen;
     Ey -= 0.2*lookVec[1]/focLen;
     Ez -= 0.2*lookVec[2]/focLen;
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   else if(ch == 'a') {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     Ex -= 0.2*rightVec[0]; //rightVec is a unit vector
     Ey -= 0.2*rightVec[1];
     Ez -= 0.2*rightVec[2];
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   else if(ch == 'd') {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     Ex += 0.2*rightVec[0];
     Ey += 0.2*rightVec[1];
     Ez += 0.2*rightVec[2];
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   glFlush();
   glutSwapBuffers();

   fov = fmod(fov, 180);
   Project(fov,w2h,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph -= 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph += 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
       dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
       dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;

   Project(fov,w2h,dim);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   Project(0, w2h, dim);
}

void moveCamera() {
  if (isPersp)
   {
	  //After gluPerspective sets up the truncated pyramid, gluLookAt puts a camera at the specified coords that uses gluPersp's matrix.
      //printf("phi = %f and theta = %f and rho = %f\n", ph, th, rh);
      /*double Ex = -1*rh*Sin(th)*Cos(ph);
      double Ey = rh*Sin(ph);
      double Ez = rh*Cos(th)*Cos(ph);*/
      gluLookAt(Ex,Ey,Ez, lookVec[0]+Ex, lookVec[1]+Ey, lookVec[2]+Ez, 0,Cos(ph),0);
      //printf("ex = %f ey = %f ez = %f\n", Ex, Ey, Ez);
      //printf("Focusing on (%f,%f,%f)\n", lookVec[0], lookVec[1], lookVec[2]);
   } else {
	  //  Set view angle
	  glRotated(ph,1,0,0);
	  glRotated(th,0,1,0);
  }
}

void display() {
  //printf("display \n");

  //Lighting variables, taken from example 26
  float Emission[]  = {0.0,0.0,0.0,1.0};
  float Ambient[]   = {0.3,0.3,0.3,1.0};
  float Diffuse[]   = {1.0,1.0,1.0,1.0};
  float Specular[]  = {1.0,1.0,1.0,1.0};
  float Position[]  = {0.9,0.9,0.9};//{2*Cos(zh),Ylight,2*Sin(zh),1.0};
  float Shinyness[] = {16};
  //This needs to be called before glClear for reasons unknown!!!!!!!!!!!
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE); //Normalizes all the normal vectors.
  glEnable(GL_LIGHTING);
  //  Clear the image
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  Reset previous transforms
  glLoadIdentity();
  moveCamera();
  //  Enable light 0
  glEnable(GL_LIGHT0);
  //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);
  //  Set materials
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

  glPushMatrix();
  drawSeaUrchin();
  //drawCube();
  glPopMatrix();

  double rad = 1.6;
  double i;
  for(i = 0; i < (2*PI); i+=(PI/4)) {
    glPushMatrix();
    glTranslated(rad*cos(i),rad*sin(i),0);
    double scalar = fabs(0.05*Sin(cubeRotate+(180*i/PI)))+0.1;
    glScaled(scalar, scalar, scalar);
    glRotated(cubeRotate, cos(i), sin(i), 0);
    drawCube();
    glPopMatrix();
  }
  glFlush();
  glutSwapBuffers(); //this is for double buffered window. Single buffered uses glFlush.
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if(shouldMove) cubeRotate = fmod(100*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   init();
   glutInit(&argc,argv);
   //  Request double buffered, true color window with z-buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE| GLUT_DEPTH);
   glEnable(GL_DEPTH_TEST);

   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Oogluong the Elder Sea Urchin (Audrey Randall)");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //obj = LoadOBJ("elf_obj.obj");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
