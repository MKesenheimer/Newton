#ifndef STAR_H
#define STAR_H

#include <vector>
#include "Object.h"

class Star : public Object {
  public:
    //the constructor
    Star(float x, float y, float vx, float vy, float size);
    
    float R();
    void set_R(float R);
    
    //the destructor
    ~Star();
  private:
    float R_;
};

#endif