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

float Object::x() {
  return x_;
}

float Object::y() {
  return y_;
}

float Object::xcenter() {
  return x_;
}

float Object::ycenter() {
  return y_;
}

float Object::vx() {
  return vx_;
}

float Object::vy() {
  return vy_;
}

float Object::phi() {
  return phi_;
}

float Object::spin() {
  return spin_;
}

float Object::size() {
  return size_;
}

int Object::npoints() {
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
    float xfs = points[i].x;
    float yfs = points[i].y;
    points[i].x = cos(dphi)*xfs - sin(dphi)*yfs;
    points[i].y = sin(dphi)*xfs + cos(dphi)*yfs;
  }
  old_phi_ = phi_;
}

void Object::set_spin(float spin) {
  spin_ = spin;
}

void Object::new_point(float x, float y, bool iscol) {
  if (npoints_ == 0) {
    //center of object
    points.push_back(point());
    points[0].index = npoints_;
    points[0].x = x;
    points[0].y = y;
    points[0].iscollidable = iscol;
    npoints_++;
  }
  else {
    points.push_back(point());
    points[npoints_].index = npoints_;
    //move all object points back into the world coordinate system
    points[npoints_].x = size_*x;
    points[npoints_].y = size_*y;
    points[npoints_].iscollidable = iscol;
    npoints_++;
  }
}

std::vector<float> Object::get_point(int n) {
  std::vector<float> retv;
  for (int i=0; i<=n; i++) {
    if (points[i].index == n) {
      retv.push_back(points[i].x + x_);
      retv.push_back(points[i].y + y_);
      return retv;
    }
  }
  std::cout<<"an error occured in Object.cpp: n="<<n<<" is not a valid index"<<std::endl;
  retv.push_back(0);
  retv.push_back(0);
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
      points[i].x = size_*x;
      points[i].y = size_*y;
    }
  }
}

void Object::update_position(float dt) {
  x_ = x_ + vx_*dt;
  y_ = y_ + vy_*dt;
  phi_ = phi_ + spin_*dt;
  set_angle(phi_);
}