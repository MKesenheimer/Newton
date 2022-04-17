#pragma once

#include "Object.h"
#include "typedefs.h"

class Collision {
  public:
    //this function takes two lists of points and checks if the
    //the objects formed by the points has collided
    //Note: the points should form an convex object at best
    bool check_collision(const Object &o1, const Object &o2);

  private:
    //calculates the center of the object formed by the points
    // returns the x- and y- coordinate in an array
    Point center(const Object &o);
    
    //returns the length of the largest distance from center
    //this assumed to be the dimension of the object
    float dim(const Object &o);
    
    //calculate the distance of two objects
    float dist(const Object &o1, const Object &o2);
    float dist(const Object &o1, const Point &point);
    float dist(const Point &point1, const Point &point2);
};