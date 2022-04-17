#include "Collision.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

bool Collision::check_collision(Object *o1, Object *o2) {
  float dim1 = dim(o1);
  float dim2 = dim(o2);
  //std::cout << "dim1 = " << dim1 << std::endl;
  //std::cout << "dim2 = " << dim2 << std::endl;
  //std::cout << "dist = " << dist(o1,o2) << std::endl;
  if(dim1+dim2>=dist(o1,o2)) {
    return true;
  } else {
    return false;
  }
}

float Collision::dist(Object *o1, Object *o2) {
  std::vector<float> cen1 = center(o1);
  std::vector<float> cen2 = center(o2);
  //calculate the distance of the two objects
  float dist = sqrt(pow(cen1[0] - cen2[0],2.)+pow(cen1[1] - cen2[1],2.));
  return dist;
}

float Collision::dist(Object *o, std::vector<float> point) {
  std::vector<float> cen = center(o);
  float dist = sqrt(pow(cen[0] - point[0],2.)+pow(cen[1] - point[1],2.));
  return dist;
}

float Collision::dist(std::vector<float> point1, std::vector<float> point2) {
  float dist = sqrt(pow(point1[0] - point2[0],2.)+pow(point1[1] - point2[1],2.));
  return dist;
}

std::vector<float> Collision::center(Object *o) {
  std::vector<float> temp;
  
  temp.push_back(o->x());
  temp.push_back(o->y());
  
  return temp;
}

float Collision::dim(Object *o) {
  return o->size();
}