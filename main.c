
#include "util.h"
/*
  Audrey Randall's homework 4
  Citations:
    example 26 from class
    http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
*/

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
     up = 1;
   }
   else if(ch == 's') {
     down = 1;
   }
   else if(ch == 'a') {
     left = 1;
   }
   else if(ch == 'd') {
     right = 1;
   }
   else if(ch == ' ') {
     ltMove = !ltMove;
   }
   else if(ch == 'i') {
     switch(ltParam){
       case 0:
         if(ambient < 1) ambient += 0.1;
         break;
       case 1:
         if(diffuse < 1) diffuse += 0.1;
         break;
       case 2:
         if(specular < 1) specular += 0.1;
       case 3:
         if(emission < 1) emission += 0.1;
         break;
       case 4:
         shinyness += 1;
         break;
      case 5:
        if(matEmit < 1) matEmit += 0.1;
        break;
      case 6:
        if(matSpec < 1) matSpec += 0.1;
        break;
     }
   }
   else if(ch == 'k') {
     switch(ltParam){
       case 0:
         if(ambient > 0) ambient -= 0.1;
         break;
       case 1:
         if(diffuse > 0) diffuse -= 0.1;
         break;
       case 2:
         if(specular > 1) specular -= 0.1;
       case 3:
         if(emission > 0) emission -= 0.1;
         break;
      case 4:
        shinyness -= 1;
        break;
      case 5:
        if(matEmit > 0) matEmit -= 0.1;
        break;
      case 6:
        if(matSpec > 0) matSpec -= 0.1;
        break;
     }
   }
   else if (ch == 'j') {
     if(ltParam < 7) ltParam++;
     else ltParam = 0;
   }
   printf("ltParam = %d\n", ltParam);
   printf("Light parameters: emission %f, ambient %f, diffuse %f, specular %f\n", emission, ambient, diffuse, specular);
   printf("Material parameters: shinyness %f, emission %f, specular %f\n", shinyness, matEmit, matSpec);
   //leaving these in makes the odd flickering thing happen. Flush and swap should only be in display.
   //glFlush();
   //glutSwapBuffers();

   fov = fmod(fov, 180);
   Project(fov,w2h,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void key_up(unsigned char ch,int x,int y)
{
  if(ch == 'w') {
    up = 0;
  }
  else if(ch == 's') {
    down = 0;
  }
  else if(ch == 'a') {
    left = 0;
  }
  else if(ch == 'd') {
    right = 0;
  }
}

void mouse_motion(int x, int y) {
  double lookRho;
  int lookPhi;
  int lookTheta;
  double rightRho;
  int rightPhi;
  int rightTheta;
  if(rotateCamera) {
    int deltaX = x-lastX;
    int deltaY = y-lastY;
    lastX = x;
    lastY = y;
    printf("deltaX = %d and deltaY = %d\n", deltaX, deltaY);
    cartesianToSpherical(lookVec[0], lookVec[1], lookVec[2], &lookRho, &lookPhi, &lookTheta);
    cartesianToSpherical(rightVec[0], rightVec[1], rightVec[2], &rightRho, &rightPhi, &rightTheta);
    lookTheta += (int)(sensitivity*(double)deltaX);
    rightTheta += (int)(sensitivity*(double)deltaX);
    printf("Rotating by %d.\n", (int)(sensitivity*(double)deltaX));
    sphericalToCartesian(lookRho, lookPhi, lookTheta, &lookVec[0], &lookVec[1], &lookVec[2]);
    sphericalToCartesian(rightRho, rightPhi, rightTheta, &rightVec[0], &rightVec[1], &rightVec[2]);
    printf("Position: (%f,%f,%f) lookVec: (%f,%f,%f) rightVec: (%f,%f,%f)\n", Ex,Ey,Ez, lookVec[0],lookVec[1],lookVec[2], rightVec[0],rightVec[1],rightVec[2]);
  }
}

void on_click(int button, int state, int x, int y)
{
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    printf("Right Button down!\n");
    lastX = x;
    lastY = y;
    rotateCamera = 1;
  }
  if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    printf("Right button up!\n");
    rotateCamera = 0;
  }
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
      gluLookAt(Ex,Ey,Ez, lookVec[0]+Ex, lookVec[1]+Ey, lookVec[2]+Ez, 0,Cos(ph),0);
   } else {
	  //  Set view angle
	  glRotated(ph,1,0,0);
	  glRotated(th,0,1,0);
  }
}

void display() {
  //This needs to be called before glClear for reasons unknown!!!!!!!!!!!
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE); //Normalizes all the normal vectors.
  glEnable(GL_LIGHTING);
  glLoadIdentity();
  moveCamera();

  glEnable(GL_LIGHT0);
  getLtPos();
  setLight();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //draw light placeholder
  glPushMatrix();
  glTranslated(ltPos[0], ltPos[1], ltPos[2]);
  glScaled(0.1,0.1,0.1);
  drawCube();
  glPopMatrix();

  glPushMatrix();
  drawSeaUrchin();
  //drawCube();
  glPopMatrix();

  //glDisable(GL_LIGHTING);
  //glDisable(GL_LIGHT0);

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
  Print("Property: %d Values: %f, %f, %f, %f", ltParam, emission, ambient, diffuse, specular);
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
   if(ltMove) ltAng = fmod(ltAng+2, 360.0);

   if(up) {
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
   if(down) {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     double focLen = sqrt((lookVec[0])*(lookVec[0])+(lookVec[1])*(lookVec[1])+(lookVec[2])*(lookVec[2]));
     printf("focLen is %f\n", focLen);
     Ex -= 0.2*lookVec[0]/focLen;
     Ey -= 0.2*lookVec[1]/focLen;
     Ez -= 0.2*lookVec[2]/focLen;
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   if(left) {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     Ex -= 0.2*rightVec[0]; //rightVec is a unit vector
     Ey -= 0.2*rightVec[1];
     Ez -= 0.2*rightVec[2];
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }
   if(right) {
     printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
     Ex += 0.2*rightVec[0];
     Ey += 0.2*rightVec[1];
     Ez += 0.2*rightVec[2];
     cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
     printf("After step: Ex, Ey, Ez => (%f,%f,%f) and focal pt (%f,%f,%f)\n", Ex, Ey, Ez, lookVec[0], lookVec[1], lookVec[2]);
   }

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
   glutKeyboardUpFunc(key_up);
   glutIdleFunc(idle);
   glutMotionFunc(mouse_motion);
   glutMouseFunc(on_click);
   //obj = LoadOBJ("elf_obj.obj");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
