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
#include "controller.h"
#include "viewer.h"

#define _USE_MATH_DEFINES
#define M_PI 3.1415926535

int main(int argc, char * argv[]) {
  float center[3] = {0.0, 0.0, 0.0};
  float color[3] = {0.0, 1.0, 0.0};
  SuperEllipse super_ellipse = SuperEllipse(1, 1, 1, .5, 5, 20, center, color);
  Controller controller = Controller(super_ellipse);
  Viewer viewer = Viewer(super_ellipse, controller);
  viewer.initGlut(argc, argv);

  return 0;
}
