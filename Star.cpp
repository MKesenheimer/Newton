#include "Star.h"
#define _USE_MATH_DEFINES
#include <math.h>

Star::Star(float x, float y, float vx, float vy, float R) : Object(x, y, vx, vy){
  R_ = R;
  setSize(2 * R, 2 * R);
}

void Star::setR(float R) {
  R_ = R;
  setSize(2 * R, 2 * R);
}

float Star::R() {
  return R_;
}