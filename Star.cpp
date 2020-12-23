#include "Star.h"
#define _USE_MATH_DEFINES
#include <math.h>

Star::Star(float x, float y, float vx, float vy, float R) : Object(x,y,vx,vy){
  //points are defined
  //in the object-own coordinate system
  R_ = R;
  new_point(0,0);
  set_size(R);
}

void Star::set_R(float R) {
  R_ = R;
  set_size(R);
}

float Star::R() {
  return R_;
}

Star::~Star() {

}