#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>

//get the angle in respect to the x-axis from a vector with x & y coordinates
float getAngle(float x, float y);

//easier print function
template <typename T>
void print(T t) {
  std::cout << t << std::endl;
}

//recursive variadic print function
template<typename T, typename ...Args>
void print(T t, Args ...args) {
  std::cout << t;
  print(args...);
}

void seed(int n);
float frand(float min, float max);

#endif
