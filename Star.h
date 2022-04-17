#pragma once

#include <vector>
#include "GameLibrary/Object.h"

class Star : public Object {
  public:
    //the constructor
    Star(float x, float y, float vx, float vy, float size);
    
    float R();
    void setR(float R);
  private:
    float R_;
};