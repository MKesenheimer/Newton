#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "SDLTools/Utilities.h"
#include "SDLTools/Timer.h"
#include "GameLibrary/Collision.h"
#include "GameLibrary/Solver.h"
#include "GameLibrary/Renderer.h"
#include "Star.h"

const int BOX_WIDTH = 20;
const int BOX_HEIGHT = 20;
//Fps auf 20 festlegen
const int FRAMES_PER_SECOND = 20;
const int NSTARS = 500;

int main( int argc, char* args[] ) {
  int frame = 0; //take records of frame number
  bool cap = true; //Framecap an oder ausschalten

  //Timer zum festlegen der FPS
  sdl::auxiliary::Timer fps;
  //Timer zum errechnen der weltweit vergangenen Zeit
  sdl::auxiliary::Timer worldtime;
  worldtime.start();

  //calculate the small time between two frames in ms
  int oldTime = 0;
  int newTime = 0;
  int dt = 1;

  //initialize random generator
  sdl::auxiliary::Utilities::seed(time(NULL));

  //Start up SDL and make sure it went ok
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    sdl::auxiliary::Utilities::logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  //Setup our window and renderer, this time let's put our window in the center
  //of the screen
  SDL_Window *window = SDL_CreateWindow("Newton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    sdl::auxiliary::Utilities::logSDLError(std::cout, "CreateWindow");
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    sdl::auxiliary::Utilities::logSDLError(std::cout, "CreateRenderer");
    sdl::auxiliary::Utilities::cleanup(window);
    SDL_Quit();
    return 1;
  }

  //generate new objects
  std::vector<Star> stars;
  int nstars = NSTARS;
  for (int i = 0; i < nstars; i++) {
    stars.push_back(Star(sdl::auxiliary::Utilities::frand(0,SCREEN_WIDTH),sdl::auxiliary::Utilities::frand(0,SCREEN_HEIGHT),sdl::auxiliary::Utilities::frand(-1,1),sdl::auxiliary::Utilities::frand(-1,1),sdl::auxiliary::Utilities::frand(1,2)));
  }

  //simulation control
  bool quit = false;

  //constants for integration
  //newtons constant of gravity
  float gamma = 6.67408e-11;
  // mass scaling factor
  float rho = 1e10;
  // critical velocity when a collision leads to a merge
  float vcrit = 100.0;
  bool collision = false;
  float mtot = 0.0, mtot_save = 0.0;

  //Our event structure
  SDL_Event e;

  while (!quit) {
    //start the fps timer
    fps.start();

    //Read any events that occured, for now we'll just quit if any event occurs
    while (SDL_PollEvent(&e)) {
      //If user closes the window
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      //If user presses any key
      if (e.type == SDL_KEYDOWN) {
        if( e.key.keysym.sym == SDLK_c ) {
          cap = !cap;
        }
      }
    }

    for (int i = 0; i < nstars; i++) {
      //mechanics
      float xi, yi, vxi, vyi, mi;
      xi = stars[i].x();
      yi = stars[i].y();
      vxi = stars[i].vx();
      vyi = stars[i].vy();
      // mass of the star, TODO: move this to a function
      mi = pow(stars[i].R(), 3) * rho;

      //std::cout << "xi = " << xi << std::endl;

      float s1 = 0., sx2 = 0., sy2 = 0.;
      for (int j = 0; j < nstars; j++) {
        if(i != j) {
          float xj, yj;
          xj = stars[j].x();
          yj = stars[j].y();

          if(xi != xj and yi != yj) {
            float rij, mj;
            // mass of the star, TODO: move this to a function
            mj = pow(stars[j].R(), 3) * rho;
            rij = sqrt(pow(xi - xj, 2.) + pow(yi - yj, 2.));
            s1  += - gamma * mi * mj / pow(rij, 3.);
            sx2 += + gamma * mi * mj * xj / pow(rij, 3.);
            sy2 += + gamma * mi * mj * yj / pow(rij, 3.);
            collision = false;
          } else {
            collision = true;
          }
        }
      }

      if(!collision) {
        // löst lineare DGL der Form x'' = ww*x + bet*x' + al.
        float wwx = s1 / mi, betx = 0., alx = sx2 / mi;
        RungeKuttaSolver::step(wwx, betx, alx, dt, &xi, &vxi);

        float wwy = s1 / mi, bety = 0., aly = sy2 / mi;
        RungeKuttaSolver::step(wwy, bety, aly, dt, &yi, &vyi);

        stars[i].setv(vxi, vyi);
        stars[i].setPos(xi, yi);
      }

      // collision detection
      for (int j = i+1; j < nstars; j++) {
        collision = Collision::checkCollision(stars[i], stars[j]);
        //std::cout << collision << std::endl;
        if(collision) {
          float xj = stars[j].x();
          float yj = stars[j].y();
          float vxj = stars[j].vx();
          float vyj = stars[j].vy();
          float mj = pow(stars[j].R(), 3.) * rho;
          float vrel = pow(pow(vxi - vxj, 2.) + pow(vyi - vyj, 2.), .5);
          //std::cout << "vrel = " << vrel << std::endl;
          //momentum conservation
          if(vrel <= vcrit) {
            xi = (xi * mi + xj * mj) / (mi + mj);
            yi = (yi * mi + yj * mj) / (mi + mj);
            vxi = (mi * vxi + mj * vxj) / (mi + mj);
            vyi = (mi * vyi + mj * vyj) / (mi + mj);
            mi = mi + mj;
            stars[i].setPos(xi, yi);
            stars[i].setv(vxi, vyi);
            float newR = pow(pow(stars[i].R(), 3.) + pow(stars[j].R(), 3.), 1/3.);
            //std::cout <<stars[i].R()<<" "<<stars[j].R()<<" "<< newR << std::endl;
            stars[i].setR(newR);
            stars.erase(stars.begin() + j);
            nstars--;
          }
          collision = false;
        }
      } 

      //mirrored boundaries
      if(xi <= 0) stars[i].setPos(SCREEN_WIDTH, yi);
      if(xi >= SCREEN_WIDTH) stars[i].setPos(0, yi);
      if(yi <= 0) stars[i].setPos(xi, SCREEN_HEIGHT);
      if(yi >= SCREEN_HEIGHT) stars[i].setPos(xi, 0);
    }

    //check total mass
    mtot_save = mtot;
    mtot = 0;
    for (int i = 0; i < nstars; i++) {
      mtot += pow(stars[i].R(), 3)*rho;
    }

    if(mtot_save != 0.0 and fabs(mtot_save-mtot)/mtot >= 0.001) {
      std::cout << "Mass not conserved. Bug?" << std::endl;
      std::cout << "mtot_save = " << mtot_save << ", mtot = " << mtot << std::endl;
      exit(0);
    }

    //Rendering
    SDL_RenderClear(renderer);
    // Draw the background black
    boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 255);

    // draw the particles
    for (int i = 0; i < nstars; i++) {
      Renderer::drawObject(stars[i], renderer);
    }

    // render
    SDL_RenderPresent(renderer);
    // Timer related stuff
    oldTime = newTime;
    newTime = worldtime.getTicks();
    if (newTime > oldTime) {
      dt = (newTime - oldTime) / 1000.; // small time between two frames in s
    }
    if(dt == 0) dt = 1;

    // increment the frame number
    frame++;
    // apply the fps cap
    if((cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND)) {
      SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.getTicks());
    }

    //update the window caption
    if( worldtime.getTicks() > 1000 ) {
      std::stringstream caption;
      caption << "Newton, FPS = " << 1000.f * frame / worldtime.getTicks() << ", nstars = " << nstars;
      SDL_SetWindowTitle(window, caption.str().c_str());
      worldtime.start();
      frame = 0;
    }
  }

  // Destroy the various items
  sdl::auxiliary::Utilities::cleanup(renderer, window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
