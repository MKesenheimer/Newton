#include <math.h>
#include "Functions.h"

float getAngle(float x, float y) {
  float theta = 0.0f;
  if ( x>0 ) {
    theta = atan(y/x);
  } else if ( x<0.0f && y>=0.0f ) {
    theta = atan(y/x) + M_PI;
  } else if ( x<0.0f && y<0.0f ) {
    theta = atan(y/x) - M_PI;
  } else if ( x==0.0f && y>0.0f ) {
    theta = M_PI/2.0f;
  } else if ( x==0.0f && y<0.0f ) {
    theta = -M_PI/2.0f;
  } else {
    theta = 0.0f;
  }
    
  return theta;
}

void seed(int n) {
  std::srand(n);
}

float frand(float min, float max) {
  return min + (max - min) * ((std::rand() % 1000) / 1e3);
}