#ifndef SUPER_ELLIPSE_H
#define SUPER_ELLIPSE_H

// An extruded superellipse in space, with adjustable major and minor axis,
// length, face angles, and number of points on each face.
class SuperEllipse {
 public:
  SuperEllipse(float a, float b, float c, float n, int k,
               float center[3], float color[3]);
  SuperEllipse(float a, float b, float c, float n, int k, int max_size,
               float center[3], float color[3]);
  void genVerticesAndIndices();
  float faceAngle(int face, char axis);
  void setFaceAngle(int face, char axis, float change);
  void clearVerticesAndIndices();

  GLfloat * vertices();
  GLubyte * indices();
  GLfloat * colors();

  float a_, b_, c_, n_;
  int k_;
  float face_1_x_angle_ = 0, face_1_y_angle_ = 0;
  float face_2_x_angle_ = 0, face_2_y_angle_ = 0;
  std::string move_mode_ = "move";

 private:
  template<typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }
  float signedPow(float x, float n);
  void genColors();
  void genVertices();
  void genIndices();

  // GLfloat * vertices_;
  // GLubyte * indices_;
  // GLfloat * colors_;

  GLfloat vertices_[25*20 + 3];
  GLubyte indices_[2*48*20];
  GLfloat colors_[25*20 + 3];

  int v_size_ = 0;
  int i_size_ = 0;
  int c_size_ = 0;
  int max_size_;

  float center_[3], color_[3];

  int draw_mode_ = 0;
  int face_selected_ = 0;
};

#endif
