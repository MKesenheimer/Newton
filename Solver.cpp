#include "Solver.h"

Solver::Solver(float x0, float v0, float ww, float bet, float al) : x_(x0), v_(v0), ww_(ww), bet_(bet), al_(al) {}

void Solver::step(float dt, float* x, float* v) {
  RungeKuttaSolver::step(ww_, bet_, al_, dt, &x_, &v_);
  *x = x_;
  *v = v_;
}

void RungeKuttaSolver::step(float ww, float bet, float al, float dt, float* x, float* v) {
  float xs = *x, vs = *v;

  // Runge-Kutta 4. Ordnung
  float k1[4], k2[4];
  k1[0] = vs;
  k2[0] = ww * xs + bet * vs + al;
  k1[1] = vs  + 0.5 * k2[0] * dt;
  k2[1] = ww * (xs + 0.5 * k1[0] * dt) + bet * (vs + 0.5 * k2[0] * dt) + al; // rho * (t + 0.5 * dt)
  k1[2] = vs  + 0.5 * k2[1] * dt;
  k2[2] = ww * (xs + 0.5 * k1[1] * dt) + bet * (vs + 0.5 * k2[1] * dt) + al; // rho * (t + 0.5 * dt)
  k1[3] = vs  + k2[2] * dt;
  k2[3] = ww * (xs + k1[2] * dt) + bet * (vs + k2[2] * dt) + al; // rho * (t + dt)
  xs   += dt  * (1./6 * k1[0] + 1./3 * k1[1] + 1./3 * k1[2] + 1./6 * k1[3]);
  vs   += dt  * (1./6 * k2[0] + 1./3 * k2[1] + 1./3 * k2[2] + 1./6 * k2[3]);
  //ax = ww * xs + bet * vs + al;

  // update x and v
  *x = xs;
  *v = vs;
}