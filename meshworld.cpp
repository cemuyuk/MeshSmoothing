//=========================================================
//
// CS 148 (Summer 2016) - Mesh Smoothing
//
//=========================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

// Includes all libst classes
#include "lib/libst/include/st.h"

const int g_k_window_width = 768;
const int g_k_window_height = 512;

// The default mesh
STShape * g_shape;

// How many iterations of smoothing
unsigned int g_k_num_iterations = 1;

// Inspection controls
bool g_just_clicked = false;
double g_mouse_x = 0.0;
double g_mouse_y = 0.0;
double g_mouse_x_prev = 0.0;
double g_mouse_y_prev = 0.0;
double g_rot_angle_x = 0.0;
double g_rot_angle_y = 0.0;

//--------------------------------------------------------- 
// making a copy of "g_shape" which models "g_k_num_iterations" of
// Laplacian smoothing.
//---------------------------------------------------------
void computeLaplacianSmoothedMesh()
{
  // Do it!
    size_t numVertices,N,tempIndex;
    set<size_t> temporarySet;
    float x,y,z;
    x=0;
    y=0;
    z=0;

    numVertices=g_shape->GetNumVertices();
    
    for(size_t i=0; i<numVertices;i++){
        
        temporarySet=g_shape->GetNeighboringVertices(i);
        N=temporarySet.size();

       // cout << "Vertice no: " << i << ". Set size:"<< N <<endl;
        for(set<size_t>::iterator itr=temporarySet.begin(); itr!=temporarySet.end(); itr++){
            tempIndex=*itr;
            x+=g_shape->GetVertex(tempIndex).position.x/N;
            y+=g_shape->GetVertex(tempIndex).position.y/N;
            z+=g_shape->GetVertex(tempIndex).position.z/N;
        }
        const STPoint3 *newPosition=new STPoint3(x,y,z);
        
        STShape::Vertex *newVertex=new STShape::Vertex(*newPosition, g_shape->GetVertex(i).normal, g_shape->GetVertex(i).texCoord );
        g_shape->SetVertex(i,*newVertex);
        x=0;
        y=0;
        z=0;
    }
    g_shape->GenerateNormals();
}

//--------------------------------------------------------- 
// Render the scene
//---------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // setup lighting
  float mat_amb_diff[] = { 0.6, 0.6, 0.6, 1.0 };
  float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  float mat_shininess[] = { 10.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  // draw
  glPushMatrix();
    glRotatef(g_rot_angle_x, 0.0, 0.0, 1.0);
    glRotatef(g_rot_angle_y, 1.0, 0.0, 0.0);
    glutSolidSphere(0.04, 20, 20);
    if (g_shape) {
      g_shape->Draw();
    }
  glPopMatrix();

  glutSwapBuffers();
  glFlush();
}

//--------------------------------------------------------- 
// Number keys 1-7 are used to toggle the iteration count
//---------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    g_shape = new STShape("bunny.obj");
  switch(key) {
    case '1':
      g_k_num_iterations = 1;
      computeLaplacianSmoothedMesh();
      break;
    case '2':
      g_k_num_iterations = 2;
      computeLaplacianSmoothedMesh();
      computeLaplacianSmoothedMesh();
      break;
    case '3':
      g_k_num_iterations = 4;
          for(int i=0;i<3;i++){
              computeLaplacianSmoothedMesh();
          }
      break;
    case '4':
      g_k_num_iterations = 8;
          for(int j=0; j<8; j++){
              computeLaplacianSmoothedMesh();
          }
      break;
    case '5':
      g_k_num_iterations = 16;
          for(int m=0; m<16; m++){
              computeLaplacianSmoothedMesh();
          }
      break;
    case '6':
      g_k_num_iterations = 32;
          for(int n=0; n<32; n++){
              computeLaplacianSmoothedMesh();
          }
      break;
    case '7':
      g_k_num_iterations = 64;
          for(int l=0; l<64; l++){
              computeLaplacianSmoothedMesh();
          }
      break;
    default:
      break;
  }

  cout << "Smoothing with " << g_k_num_iterations << " iterations." << endl;
    
  
  glutPostRedisplay();
}

//--------------------------------------------------------- 
// Mouse click handler
//---------------------------------------------------------
void mouseFunc(int button, int state, int x, int y)
{
  //printf("mouse func: (%d, %d, %d, %d)\n", button, state, x, y);

  bool clicked = (state == 0);
  bool released = (state == 1);

  if (clicked) {
    if (!g_just_clicked) {
      g_mouse_x_prev = (float)x;
      g_mouse_y_prev = (float)y;
      g_just_clicked = true;
    }
  } else if (released) {
    g_just_clicked = false;
  }
}

//--------------------------------------------------------- 
// Mouse motion handler
//---------------------------------------------------------
void motionFunc(int x, int y)
{
  if (!g_just_clicked) {
    g_mouse_x_prev = g_mouse_x;
    g_mouse_y_prev = g_mouse_y;
  } else {
    g_just_clicked = false;
  }

  g_mouse_x = (float)x;
  g_mouse_y = (float)y;

  float delta_x = -0.25 * (g_mouse_x - g_mouse_x_prev);
  float delta_y =  0.25 * (g_mouse_y - g_mouse_y_prev);

  g_rot_angle_x += delta_x;
  g_rot_angle_y += delta_y;

  if (g_rot_angle_x < -360.0) g_rot_angle_x += 360.0;
  if (g_rot_angle_x > 360.0) g_rot_angle_x -= 360.0;
  if (g_rot_angle_y < -360.0) g_rot_angle_y += 360.0;
  if (g_rot_angle_y > 360.0) g_rot_angle_y -= 360.0;

  glutPostRedisplay();
}

//--------------------------------------------------------- 
// Create the mesh and initialize OpenGL state
//---------------------------------------------------------
void setupScene()
{
    //g_shape=STShapes::CreateCylinder(.1,.05,6,6);
  g_shape = new STShape("bunny.obj");
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);

  float light_pos[] = { 2.0, 3.0, 4.0, 1.0 };
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  glEnable(GL_DEPTH_TEST);

  // Setup the view of the mesh
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 60.0, /* field of view in degrees */
                  double(g_k_window_width)/double(g_k_window_height), // aspect ratio
                  0.01,  /* z near */ 
                  10.0); /* z far */ 

  glMatrixMode(GL_MODELVIEW);
  gluLookAt(-0.1, 0.16, 0.25,  /* eye is at (x,y,z) */
             0.0,  0.1,  0.0,  /* center of world */
             0.0,  1.0,  0.0);  /* up is in positive Y direction */
}

//--------------------------------------------------------- 
// Entry point
//---------------------------------------------------------
int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(g_k_window_width, g_k_window_height);

  glutCreateWindow("CS 148 - HW 5 - Laplace Smoothing");

  glutDisplayFunc(display);
  glutMouseFunc(mouseFunc);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motionFunc);
        
  setupScene();

  glutMainLoop();
    

  return 0;
}

