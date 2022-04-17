#pragma once

// solves second order differential equations: x'' = ww * x + bet * x' + al
class Solver {
  public:
    Solver(float x0, float v0, float ww, float bet, float al);

    void step(float dt, float* x, float* v);

  private:
    float x_, v_;
    const float ww_, bet_, al_;
};

class RungeKuttaSolver {
  public:
    static void step(float ww, float bet, float al, float dt, float* x, float* v);
};