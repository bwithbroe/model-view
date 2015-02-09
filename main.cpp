#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "super_ellipse.h"

#define _USE_MATH_DEFINES
#define M_PI 3.1415926535

// Windows compile: g++ -std=c++11 -o super_ellipse -Wall super_ellpse.cpp main.cpp -mwindows -lglut32 -lopengl32 -lglu32

void display();
void specialKeys();

double rotate_x=0, rotate_y=0;
double xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0;

int drawMode = 0;
int face_to_change = 1;
float center[3] = {0.0, 0.0, 0.0};
float color[3] = {0.0, 1.0, 0.0};
SuperEllipse super_ellipse = SuperEllipse(1, 1, 1, .5, 5, 20, center, color);

void cube() {
  super_ellipse.genVerticesAndIndices();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, super_ellipse.vertices());
  glColorPointer(3, GL_FLOAT, 0, super_ellipse.colors());

  glPushMatrix();
  glDrawElements(GL_TRIANGLES, 4*3*(super_ellipse.k_*8), GL_UNSIGNED_BYTE, super_ellipse.indices());
  glPopMatrix();

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void camera() {
  glRotated(xrot,1.0,0.0,0.0);  //rotate our camera on teh x-axis (left and right)
  glRotated(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
  glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display() {
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity(); // Reset transformations

  camera();
  glEnable(GL_DEPTH_TEST); //enable the depth testing
  // glEnable(GL_LIGHTING); //enable the lighting
  // glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
  // glShadeModel (GL_SMOOTH); //set the shader to smooth shader

  // Rotate when user changes rotate_x and rotate_y
  glRotatef(rotate_x, 1.0, 0.0, 0.0 );
  glRotatef(rotate_y, 0.0, 1.0, 0.0 );

  cube();

  glFlush();
  glutSwapBuffers();
}

void reshape (int w, int h) {
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 0.1, 200.0);
  glMatrixMode(GL_MODELVIEW);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
  if(super_ellipse.move_mode_ == "rotate") {
    //  Right arrow - increase rotation by 2 degree
    if(key == GLUT_KEY_RIGHT) {
      rotate_y += 2;
      //  Left arrow - decrease rotation by 2 degree
    } else if(key == GLUT_KEY_LEFT) {
      rotate_y -= 2;

    } else if(key == GLUT_KEY_UP) {
      rotate_x -= 2;

    } else if(key == GLUT_KEY_DOWN) {
      rotate_x += 2;
    }

  } else if(super_ellipse.move_mode_ == "move") {
    if(key == GLUT_KEY_UP) {
      float xrotrad, yrotrad;
      yrotrad = (yrot / 180 * 3.141592654f);
      xrotrad = (xrot / 180 * 3.141592654f);
      xpos += float(sin(yrotrad)) / 4;
      zpos -= float(cos(yrotrad)) / 4;
      ypos -= float(sin(xrotrad)) / 4;

    } else if(key == GLUT_KEY_DOWN) {
      float xrotrad, yrotrad;
      yrotrad = (yrot / 180 * 3.141592654f);
      xrotrad = (xrot / 180 * 3.141592654f);
      xpos -= float(sin(yrotrad)) / 4;
      zpos += float(cos(yrotrad)) / 4;
      ypos += float(sin(xrotrad)) / 4;

    } else if(key == GLUT_KEY_RIGHT) {
      yrot += 1;
      if(yrot >360) {
        yrot -= 360;
      }
    } else if(key == GLUT_KEY_LEFT) {
      yrot -= 1;
      if(yrot < -360){
        yrot += 360;
      }
    }

  } else if(super_ellipse.move_mode_ == "faces") {
    if(face_to_change == 1) {
      if(key == GLUT_KEY_RIGHT) {
        if(super_ellipse.faceAngle(1, x) < M_PI / 4) {
          super_ellipse.setFaceAngle(1, x, M_PI / 24);
        }
      } else if(key == GLUT_KEY_LEFT) {
        if(super_ellipse.faceAngle(1, x) > -M_PI / 4) {
          super_ellipse.setFaceAngle(1, x, -M_PI / 24);
        }
      } else if(key == GLUT_KEY_UP) {
        if(super_ellipse.faceAngle(1, y) < M_PI / 4) {
          super_ellipse.setFaceAngle(1, y, M_PI / 24);
        }
      } else if(key == GLUT_KEY_DOWN) {
        if(super_ellipse.faceAngle(1, y) > -M_PI / 4) {
          super_ellipse.setFaceAngle(1, y, -M_PI / 24);
        }
      }
    } else if(face_to_change == 2){
      if(key == GLUT_KEY_RIGHT) {
        if(super_ellipse.faceAngle(2, x) < M_PI / 4) {
          super_ellipse.setFaceAngle(2, x, M_PI / 24);
        }
      } else if(key == GLUT_KEY_LEFT) {
        if(super_ellipse.faceAngle(2, x) > -M_PI / 4) {
          super_ellipse.setFaceAngle(2, x, -M_PI / 24);
        }
      } else if(key == GLUT_KEY_UP) {
        if(super_ellipse.faceAngle(2, y) < M_PI / 4) {
          super_ellipse.setFaceAngle(2, y, M_PI / 24);
        }
      } else if(key == GLUT_KEY_DOWN) {
        if(super_ellipse.faceAngle(2, y) > -M_PI / 4) {
          super_ellipse.setFaceAngle(2, y, -M_PI / 24);
        }
      }
    }

  } else if(super_ellipse.move_mode_ == "size") {
    if(key == GLUT_KEY_UP) {
      if(super_ellipse.b_ < 6) {
        super_ellipse.b_ += .1;
      }
    } else if(key == GLUT_KEY_DOWN) {
      if(super_ellipse.b_ > .2) {
        super_ellipse.b_ -= .1;
      }
    } else if(key == GLUT_KEY_RIGHT) {
      if(super_ellipse.a_ < 6) {
        super_ellipse.a_ += .1;
      }
    } else if(key == GLUT_KEY_LEFT) {
      if(super_ellipse.a_ > .2) {
        super_ellipse.a_ -= .1;
      }
    } else if(key == GLUT_KEY_PAGE_UP) {
      if(super_ellipse.c_ < 8) {
        super_ellipse.c_ += .1;
      }
    } else if(key == GLUT_KEY_PAGE_DOWN) {
      if(super_ellipse.c_ > .2) {
        super_ellipse.c_ -= .1;
      }
    }

  } else if(super_ellipse.move_mode_ == "vertices") {
    if(key == GLUT_KEY_UP) {
      if(super_ellipse.k_ < 20) {
        ++super_ellipse.k_;
      }
    } else if(key == GLUT_KEY_DOWN) {
      if(super_ellipse.k_ > 2) {
        super_ellipse.clearVerticesAndIndices();
        --super_ellipse.k_;
      }
    } else if(key == GLUT_KEY_RIGHT) {
      if(super_ellipse.n_ < 6) {
        super_ellipse.n_ += .1;
      }
    } else if(key == GLUT_KEY_LEFT) {
      if(super_ellipse.n_ > .3) {
        super_ellipse.n_ -= .1;
      }
    }
  }

  //  Request display update
  glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y) {
  if(key == 27) {
    exit(0);
  }

  if(key == 'r') {
    super_ellipse.move_mode_ = "rotate";
  } else if(key == 'm') {
    super_ellipse.move_mode_ = "move";
  } else if(key == 'f') {
    super_ellipse.move_mode_ = "faces";
  } else if(key == 'v') {
    super_ellipse.move_mode_ = "vertices";
  } else if(key == 's') {
    super_ellipse.move_mode_ = "size";

  } else if(key == 'q') {
    xrot += 1;
    if(xrot >360) {
      xrot -= 360;
    }
  } else if(key == 'z') {
    xrot -= 1;
    if(xrot < -360) {
      xrot += 360;
    }

  } else if(key == '1') {
    face_to_change = 1;
  } else if(key == '2') {
    face_to_change = 2;

  } else if(key == 32) {
    drawMode = (drawMode + 1) % 3;
    if(drawMode == 0) {       // fill mode
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
    }
    else if(drawMode == 1) { // wireframe mode
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
    }
    else {                   // point mode
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
    }
  }
}

int main(int argc, char* argv[]) {
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);

  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Create window
  glutCreateWindow("Awesome Cube");

  //  Enable Z-buffer depth test
  glClearDepth(1.0f);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  // Callback functions
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glutReshapeFunc(reshape);

  //  Pass control to GLUT for events
  glutMainLoop();

  //  Return to OS
  return 0;
}
