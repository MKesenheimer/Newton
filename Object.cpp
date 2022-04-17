#include "Object.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

Object::Object(float x, float y, float vx, float vy, float size, float angle, float spin) {
  x_ = x;
  y_ = y;
  vx_ = vx;
  vy_ = vy;
  size_ = size;
  old_phi_ = 0.0;
  phi_ = angle;
  spin_ = spin;
  npoints_ = 0;
  new_point(x_,y_);
}

float Object::x() const {
  return x_;
}

float Object::y() const {
  return y_;
}

float Object::xcenter() const {
  return x_;
}

float Object::ycenter() const {
  return y_;
}

float Object::vx() const {
  return vx_;
}

float Object::vy() const {
  return vy_;
}

float Object::phi() const {
  return phi_;
}

float Object::spin() const {
  return spin_;
}

float Object::size() const {
  return size_;
}

int Object::npoints() const {
  return npoints_;
}

void Object::set_size(float size) {
  size_ = size;
}

void Object::set_pos(float x, float y) {
  x_ = x;
  y_ = y;
}

void Object::set_v(float vx, float vy) {
  vx_ = vx;
  vy_ = vy;
}

void Object::set_angle(float angle) {
  phi_ = angle;
  //this algorithm rotates the object every time it is called
  //so we have to rotate only by the difference of  the old and the
  //new angle
  float dphi = (phi_ - old_phi_); //angle in rad /360.0*(2*M_PI)
  for (int i=0; i<npoints_; i++) {
    //rotate all points
    float xfs = points[i].p[0];
    float yfs = points[i].p[1];
    points[i].p[0] = cos(dphi)*xfs - sin(dphi)*yfs;
    points[i].p[1] = sin(dphi)*xfs + cos(dphi)*yfs;
  }
  old_phi_ = phi_;
}

void Object::set_spin(float spin) {
  spin_ = spin;
}

void Object::new_point(float x, float y, bool iscol) {
  if (npoints_ == 0) {
    //center of object
    points.push_back(indexablePoint());
    points[0].index = npoints_;
    points[0].p[0] = x;
    points[0].p[1] = y;
    points[0].iscollidable = iscol;
    npoints_++;
  }
  else {
    points.push_back(indexablePoint());
    points[npoints_].index = npoints_;
    //move all object points back into the world coordinate system
    points[npoints_].p[0] = size_*x;
    points[npoints_].p[1] = size_*y;
    points[npoints_].iscollidable = iscol;
    npoints_++;
  }
}

Point Object::get_point(int n) {
  Point retv;
  for (int i=0; i<=n; i++) {
    if (points[i].index == n) {
      retv[0] = points[i].p[0] + x_;
      retv[1] = points[i].p[1] + y_;
      return retv;
    }
  }
  std::cout<<"an error occured in Object.cpp: n="<<n<<" is not a valid index"<<std::endl;
  retv[0] = 0;
  retv[1] = 0;
  return retv;
}

bool Object::is_collidable(int n) {
  for (int i=0; i<=n; i++) {
    if (points[i].index == n) {
      return points[i].iscollidable;
    }
  }
  std::cout<<"an error occured in Object.cpp: n="<<n<<" is not a valid index"<<std::endl;
  return true;
}

void Object::modify_point(float x, float y, int n) {
  for (int i=0; i<=n; i++) {
    if (points[i].index == n) {
      points[i].p[0] = size_*x;
      points[i].p[1] = size_*y;
    }
  }
}

void Object::update_position(float dt) {
  x_ = x_ + vx_*dt;
  y_ = y_ + vy_*dt;
  phi_ = phi_ + spin_*dt;
  set_angle(phi_);
}