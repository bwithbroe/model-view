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
// #define max_size_ 20
#define M_PI 3.1415926535

SuperEllipse::SuperEllipse(float a, float b, float c, float n, int k,
float center[3], float color[3]) {
  a_ = a;
  b_ = b;
  c_ = c;
  n_ = n;
  k_ = k;
  for(int i = 0; i < 3; ++i) {
    center_[i] = center[i];
    color_[i] = color[i];
  }

  // vertices_ = new GLfloat[25*k_ + 3];
  // indices_ = new GLubyte[96*k_];
  // colors_ = new GLfloat[25*k_ + 3];

  genVertices();
  genIndices();
  genColors();
}

SuperEllipse::SuperEllipse(float a, float b, float c, float n, int k,
int max_size, float center[3], float color[3]) {
  a_ = a;
  b_ = b;
  c_ = c;
  n_ = n;
  k_ = k;
  max_size_ = max_size;
  for(int i = 0; i < 3; ++i) {
    center_[i] = center[i];
    color_[i] = color[i];
  }

  // vertices_ = new GLfloat[25*k_ + 3];
  // indices_ = new GLubyte[96*k_];
  // colors_ = new GLfloat[25*k_ + 3];

  genVertices();
  genIndices();
  genColors();
}

void SuperEllipse::genVerticesAndIndices() {
  genVertices();
  genIndices();
}

void SuperEllipse::genColors() {
  for (float i = 1; i <= 24*k_ + 2; i++) {
    colors_[c_size_++] = (1/i)/2 +.5;
  }
  c_size_ = 0;
}

// Signed pow function for use in the vertex generation
float SuperEllipse::signedPow(float x, float n) {
  float i = sgn(x);
  x = fabs(x);
  return i * pow(x, n);
}

// gen all the exterior points of the superellipse end-caps, as well as a center_ point
// for each.
void SuperEllipse::genVertices() {
  v_size_ = 0;
  c_size_ = 0;

  float x_placement_ = 0;
  float y_placement_ = 0;

  int face_to_draw_ = 1;

  //gen p vertices_
  for(float offset = -c_; offset <= c_ + 1e-6; offset += 2.0*c_) {
    vertices_[v_size_++] = center_[0];
    colors_[c_size_++] = 1;
    vertices_[v_size_++] = center_[1];
    colors_[c_size_++] = 0;
    vertices_[v_size_++] = offset;
    colors_[c_size_++] = center_[2];

    for(float i = 0; i < 4*k_; i += 1.0) {
      x_placement_ = a_*signedPow(cos((i*M_PI)/(2.0*k_)), 2.0/n_) + center_[0];
      vertices_[v_size_++] = x_placement_;
      //printf("vertices_[%i] = %f\n", v_size_-1, vertices_[v_size_-1]);
      colors_[c_size_++] = 1;

      y_placement_ = b_*signedPow(sin((i*M_PI)/(2.0*k_)), 2.0/n_) + center_[1];
      vertices_[v_size_++] = y_placement_;
      //printf("vertices_[%i] = %f\n", v_size_-1, vertices_[v_size_-1]);
      colors_[c_size_++] = 0;

      if(face_to_draw_ == 1) {
        vertices_[v_size_++] = offset + (x_placement_ / a_) * tan(face_2_x_angle_) + (y_placement_ / b_) * tan(face_2_y_angle_);
        // printf("vertices[%i] = %f\n", v_size-1, vertices[v_size-1]);
        colors_[c_size_++] = 0;
      } else {
        vertices_[v_size_++] = offset + (x_placement_ / a_) * tan(face_1_x_angle_) + (y_placement_ / b_) * tan(face_1_y_angle_);
        //printf("vertices[%i] = %f\n", v_size-1, vertices[v_size-1]);
        colors_[c_size_++] = 0;
      }
    }
    ++face_to_draw_;
  }
}

void SuperEllipse::genIndices() {
  i_size_ = 0;

  unsigned int top_ind = 4*k_;
  unsigned int q = top_ind + 1;
  for(unsigned int i = 1; i <= top_ind; ++i) {
    unsigned int next = (i % top_ind) + 1;
    indices_[i_size_++] = next;
    indices_[i_size_++] = i;
    indices_[i_size_++] = q + next;

    indices_[i_size_++] = q + next;
    indices_[i_size_++] = i;
    indices_[i_size_++] = q + i;

    indices_[i_size_++] = 0;
    indices_[i_size_++] = i;
    indices_[i_size_++] = next;

    indices_[i_size_++] = q;
    indices_[i_size_++] = q + i;
    indices_[i_size_++] = q + next;
    //    printf("Tris: [%i,%i,%i] [%i,%i,%i] [%i,%i,%i] [%i,%i,%i]\n", indices_[i_size_-12], indices_[i_size_-11], indices_[i_size_-10], indices_[i_size_-9], indices_[i_size_-8], indices_[i_size_-7], indices_[i_size_-6], indices_[i_size_-5], indices_[i_size_-4], indices_[i_size_-3], indices_[i_size_-2], indices_[i_size_-1]);
  }
}

GLfloat * SuperEllipse::vertices() {
  return vertices_;
}

GLubyte * SuperEllipse::indices() {
  return indices_;
}

GLfloat * SuperEllipse::colors() {
  return colors_;
}

float SuperEllipse::faceAngle(int face, char axis) {
  if(face == 1) {
    if(axis == 'x') {
      return face_1_x_angle_;
    } else if(axis == 'y') {
      return face_1_y_angle_;
    }
  } else if(face == 2) {
    if(axis == 'x') {
      return face_2_x_angle_;
    } else if(axis == 'y') {
      return face_2_y_angle_;
    }
  }
  return 0;
}

void SuperEllipse::setFaceAngle(int face, char axis, float change) {
  if(face == 1) {
    if(axis == 'x') {
      face_1_x_angle_ += change;
    } else if(axis == 'y') {
      face_1_y_angle_ += change;
    }
  } else if(face == 2) {
    if(axis == 'x') {
      face_2_x_angle_ += change;
    } else if(axis == 'y') {
      face_2_y_angle_ += change;
    }
  }
}

void SuperEllipse::clearVerticesAndIndices() {
  for(int i = 0; i < 25*k_ + 3; ++i) {
    vertices_[i] = 0;
  }
  for(int i = 0; i < 96*k_; ++i) {
    indices_[i] = 0;
  }
}
