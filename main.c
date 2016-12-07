
#include "util.h"
#include "shaders.h"
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
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim*7,+dim*7);
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
	 //fov=0;
    drawLight = !drawLight;
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
   } else if (ch == 'z'){
     double x;
     double y;
     double z;
     sphericalToCartesian(1.0, 45, 45, &x, &y, &z);
     printf("(%f,%f,%f) should be (0.5,0.5,0.707)\n", x, y, z);
   }
   printf("ltParam = %d\n", ltParam);
   printf("Light parameters: emission %f, ambient %f, diffuse %f, specular %f\n", emission, ambient, diffuse, specular);
   printf("Material parameters: shinyness %f, emission %f, specular %f\n", shinyness, matEmit, matSpec);
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
    double lookLen = sqrt(lookVec[0]*lookVec[0]+lookVec[1]*lookVec[1]+lookVec[2]*lookVec[2]);
    double rightLen = sqrt(rightVec[0]*rightVec[0]+rightVec[1]*rightVec[1]+rightVec[2]*rightVec[2]);
    sphericalToCartesian(lookRho/lookLen, lookPhi/lookLen, lookTheta/lookLen, &lookVec[0], &lookVec[1], &lookVec[2]);
    sphericalToCartesian(rightRho/rightLen, rightPhi/rightLen, rightTheta/rightLen, &rightVec[0], &rightVec[1], &rightVec[2]);
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
void reshape(int w,int h)
{
   //  Ratio of the width to the height of the window
   w2h = (h>0) ? (double)w/h : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, w,h);
   //  Tell OpenGL we want to manipulate the projection matrix
   Project(0, w2h, dim);
   width = w;
   height = h;
}

void moveCamera() {
  if (isPersp)
   {
    float up;
    if(Cos(ph) > 0) up = 1;
    else up = -1;
    gluLookAt(Ex,Ey,Ez, lookVec[0]+Ex, lookVec[1]+Ey, lookVec[2]+Ez, 0,1,0);

   } else {
	  //  Set view angle
	  glRotated(ph,1,0,0);
	  glRotated(th,0,1,0);
  }
}

/*
 *  Draw sky box
 */
 void Sky(double D)
{
	 setUniforms(shader_texture, frameInSec, sky[0]);
	 glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   //glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
	 glNormal3f(0,0,1);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

	 glNormal3f(-1,0,0);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

	 glNormal3f(0,0,-1);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

	 glNormal3f(1,0,0);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   //glBindTexture(GL_TEXTURE_2D,sky[1]);
	 setUniforms(shader_texture, frameInSec, sky[1]);
   glBegin(GL_QUADS);
	 glNormal3f(0,-1,0);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

	 glNormal3f(0,1,0);
   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   //glDisable(GL_TEXTURE_2D);
}

void display() {
  //This needs to be called before glClear for reasons unknown!!!!!!!!!!!
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE); //Normalizes all the normal vectors.
  glEnable(GL_LIGHTING);
  glLoadIdentity();
  glEnable(GL_LIGHT0);
  moveCamera();
  getLtPos();
  setLight();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0); //Should unbind frame buffer
  glViewport(0,0,width, height);
  glUseProgram(shader_uw);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  //draw light placeholder
  if(drawLight) {
    glPushMatrix();
    glTranslated(ltPos[0], ltPos[1], ltPos[2]);
    glScaled(0.1,0.1,0.1);
    drawCube();
    glPopMatrix();
  }

  //Render underwater components of scene to frame buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, fbuf);
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

  glPushMatrix();
  glTranslated(3,0,0);
  glRotated(180, 0,1,0);
  glScaled(2,2,2);
  drawRoom();
  glPopMatrix();

  glBindFramebuffer(GL_FRAMEBUFFER, 0); //Should unbind frame buffer
  glViewport(0,0,width, height);

  glUseProgram(shader_texture);
  glDisable(GL_DEPTH_TEST); //Disable depth test to draw sky box and underwater plane: rely on painter's algorithm
  glEnable(GL_CULL_FACE);
  Sky(15.0);
  glDisable(GL_CULL_FACE);

  //Render texture in frame buffer to quad the size of the screen, using the shader that causes distortion
  //If nothing is behind the stairs allow those pixels to be transparent
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shader_distort);
  //glClear(GL_COLOR_BUFFER_BIT);
  setUniforms(shader_distort, frameInSec, 0);
  glPushMatrix();
  //the size of the screen?
  drawPlane(2,2,10);
  glPopMatrix();

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(shader_uw);
  setUniforms(shader_uw, frameInSec, 0);

  glPushMatrix();
  glTranslated(3,0,0);
  glRotated(180,0,1,0);
  glScaled(2,2,2);
  drawAWStairWall();
  glPopMatrix();

  /*glPushMatrix();
  //glTranslated(0,1,0);
  glutSolidTeapot(0.7);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0,-1,0);
  glutSolidTeapot(0.7);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0,-2,0);
  glutSolidTeapot(0.7);
  glPopMatrix();*/



//Draw water last since it has to be partially transparent
  //glBindTexture(GL_TEXTURE_2D, tex_ws);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shader_ws);
  setUniforms(shader_ws, frameInSec, 0);
  glPushMatrix();
  glScaled(2,2,2);
  drawWater();
  glPopMatrix();

  glDisable(GL_BLEND);
  //TODO: need to destroy frame buffer
  glUseProgram(shader_uw);
  glDisable(GL_LIGHTING);
  glFlush();
  glutSwapBuffers(); //this is for double buffered window. Single buffered uses glFlush.
}

/*
 *  GLUT calls this routine when the window is resized
 */
 /*TODO: Rho, phi, and theta are being calculated incorrectly when we switch from cartesian to spherical coordinates.
        There might be a bug in the conversion functions, or rho, phi, and theta might not be getting updated with every press of a movement key.
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if((t-frame) <= (1./60.)) {
     return;
   } else {
     frame = t;
     if(frameInSec < 180) frameInSec++;
     else frameInSec = 0;
     if(shouldMove) cubeRotate = fmod(100*t,360.0);
     if(ltMove) ltAng = fmod(ltAng+2, 360.0);

     if(up) {
       printf("Before sphToCart: Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       //sphericalToCartesian(rh, ph, th, &Ex, &Ey, &Ez);
       printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       printf("LookVec: (%f,%f,%f)\n", lookVec[0], lookVec[1], lookVec[2]);
       //normalize(&lookVec[0], &lookVec[1], &lookVec[2]);
       Ex += 0.2*lookVec[0];
       Ey += 0.2*lookVec[1];
       Ez += 0.2*lookVec[2];
       cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
       //printf("After step:\n\t Position vector => (%f,%f,%f)\n\tFocal pt = (%f,%f,%f)\n\t Rho = %f, phi = %f\n", Ex, Ey, Ez, Ex+lookVec[0], Ey+lookVec[1], Ez+lookVec[2], rh, ph);
     }
     if(down) {
       //sphericalToCartesian(rh, ph, th, &Ex, &Ey, &Ez);
       printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       printf("LookVec: (%f,%f,%f)\n", lookVec[0], lookVec[1], lookVec[2]);
       //normalize(&lookVec[0], &lookVec[1], &lookVec[2]);
       Ex -= 0.2*lookVec[0];
       Ey -= 0.2*lookVec[1];
       Ez -= 0.2*lookVec[2];
       cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
       //printf("After step:\n\t Position vector => (%f,%f,%f)\n\tFocal pt = (%f,%f,%f)\n\t Rho = %f, phi = %f\n", Ex, Ey, Ez, Ex+lookVec[0], Ey+lookVec[1], Ez+lookVec[2], rh, ph);
     }
     if(left) {
       printf("Before sphToCart: Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       //sphericalToCartesian(rh, ph, th, &Ex, &Ey, &Ez);
       printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       printf("RightVec: (%f,%f,%f)\n", rightVec[0], rightVec[1], rightVec[2]);
       //normalize(&rightVec[0], &rightVec[1], &rightVec[2]);
       Ex -= 0.2*rightVec[0]; //rightVec is a unit vector
       Ey -= 0.2*rightVec[1];
       Ez -= 0.2*rightVec[2];
       cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
       //printf("After step:\n\t Position vector => (%f,%f,%f)\n\tFocal pt = (%f,%f,%f)\n\t Rho = %f, phi = %f\n", Ex, Ey, Ez, Ex+lookVec[0], Ey+lookVec[1], Ez+lookVec[2], rh, ph);
     }
     if(right) {
       //sphericalToCartesian(rh, ph, th, &Ex, &Ey, &Ez);
       printf("Ex, Ey, Ez: (%f,%f,%f)\n", Ex, Ey, Ez);
       printf("RightVec: (%f,%f,%f)\n", rightVec[0], rightVec[1], rightVec[2]);
       //normalize(&rightVec[0], &rightVec[1], &rightVec[2]);
       Ex += 0.2*rightVec[0];
       Ey += 0.2*rightVec[1];
       Ez += 0.2*rightVec[2];
       cartesianToSpherical(Ex, Ey, Ez, &rh, &ph, &th);
       //printf("After step:\n\t Position vector => (%f,%f,%f)\n\tFocal pt = (%f,%f,%f)\n\t Rho = %f, phi = %f\n", Ex, Ey, Ez, Ex+lookVec[0], Ey+lookVec[1], Ez+lookVec[2], rh, ph);
     }

     //  Tell GLUT it is necessary to redisplay the scene
     glutPostRedisplay();
   }
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
   width = 500;
   height = 500;
   glutInitWindowSize(width, height);
   //  Create the window
   glutCreateWindow("Flooded Ruins (Audrey Randall)");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutKeyboardUpFunc(key_up);
   glutIdleFunc(idle);
   //glutMotionFunc(mouse_motion);
   //glutMouseFunc(on_click);
   //obj = LoadOBJ("elf_obj.obj");
   
   #ifdef USEGLEW
        glewInit();
   	/*if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
   		printf("Ready for GLSL\n");
   	else {
   		printf("No GLSL support\n");
   		exit(1);
   	}*/
    #endif
    //Textures
    glBindTexture(GL_TEXTURE_2D, tex_ws);
    glActiveTexture(GL_TEXTURE1);
    tex_ws = LoadTexBMP("textures/water_tex_1.bmp");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, norm_ws);
    norm_ws = LoadTexBMP("textures/water_normals.bmp");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, tex_uw);
    tex_uw = LoadTexBMP("textures/texture_4.bmp");
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, norm_uw);
    norm_uw = LoadTexBMP("textures/normal_4.bmp");
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, sky[0]);
    sky[0] = LoadTexBMP("textures/nightsky0.bmp");
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, sky[1]);
    sky[1] = LoadTexBMP("textures/nightsky1.bmp");
    printf("Texture numbers: %d, %d, %d, %d, %d, %d\n", tex_ws, norm_ws, tex_uw, norm_uw, sky[0], sky[1]);

    //Shaders
    shader_uw = CreateShaderProg("underwater.vert","underwater.frag"); //uw = underwater
    shader_ws  = CreateShaderProg("water_surf.vert", "water_surf.frag"); //ws = water surface
    shader_distort = CreateShaderProg("distort.vert", "distort.frag"); //for stairs with no tex coords yet
    shader_debug = CreateShaderProg("debug.vert", "debug.frag");
    shader_texture = CreateShaderProg("texture.vert", "texture.frag");

    if(renderToFrameBuf() < 0) Fatal("frame buffer error\n");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
