#include "Collision.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

bool Collision::check_collision(const Object &o1, const Object &o2) {
  float dim1 = dim(o1);
  float dim2 = dim(o2);
  //std::cout << "dim1 = " << dim1 << std::endl;
  //std::cout << "dim2 = " << dim2 << std::endl;
  //std::cout << "dist = " << dist(o1,o2) << std::endl;
  if(dim1 + dim2 >= dist(o1, o2)) {
    return true;
  } else {
    return false;
  }
}

float Collision::dist(const Object &o1, const Object &o2) {
  Point cen1 = center(o1);
  Point cen2 = center(o2);
  //calculate the distance of the two objects
  float dist = sqrt(pow(cen1[0] - cen2[0], 2.) + pow(cen1[1] - cen2[1], 2.));
  return dist;
}

float Collision::dist(const Object &o, const Point &point) {
  Point cen = center(o);
  float dist = sqrt(pow(cen[0] - point[0], 2.)+pow(cen[1] - point[1], 2.));
  return dist;
}

float Collision::dist(const Point &point1, const Point &point2) {
  float dist = sqrt(pow(point1[0] - point2[0], 2.) + pow(point1[1] - point2[1], 2.));
  return dist;
}

Point Collision::center(const Object &o) {
  Point temp = {o.x(), o.y()};
  return temp;
}

float Collision::dim(const Object &o) {
  return o.size();
}