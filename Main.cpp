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

#include "SDL2_own.h"
#include "Main.h"
#include "Timer.h"
#include "Star.h"
#include "cleanup.h"
#include "Functions.h"
#include "Collision.h"

using namespace::std;

const int SCREEN_WIDTH  = 900;
const int SCREEN_HEIGHT = 800;
const int FRAMES_PER_SECOND = 20;			//Fps auf 20 festlegen
const int NSTARS = 500;

//Log an SDL error with some error message to the output stream of our choice
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

//Log an SDL debug output with some message to the output stream of our choice
void logSDLDebug(std::ostream &os, const std::string &msg){
	os << " [DEBUG]: " << msg << std::endl;
}
void logSDLDebug(std::ostream &os, const int msg){
	os << " [DEBUG]: " << msg << std::endl;
}

// Draw an Object to a SDL_Rederer
void drawObject(Object *object, SDL_Renderer *ren) {
    std::vector<float> temp;
    temp = object->get_point(1);
    int xp_old = (int)(temp[0]);
    int yp_old = (int)(temp[1]);
    for(int i=1; i<object->npoints(); i++) {
        temp = object->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        filledEllipseRGBA(ren, xp, yp, 0, 0, 0, 0, 0, 255);
        lineRGBA(ren, xp, yp, xp_old, yp_old, 0, 0, 0, 255);
        xp_old = xp;
        yp_old = yp;
    }
}

// Draw star to a SDL_Rederer
void drawStar(Object *object, SDL_Renderer *ren) {
    std::vector<float> temp;
    temp = object->get_point(1);
    int size = (int)object->size();
    for(int i=1; i<object->npoints(); i++) {
        temp = object->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        filledEllipseRGBA(ren, xp, yp, size, size, 0, 0, 0, 255);
    }
}

int main( int argc, char* args[] ) {

	int frame = 0; //take records of frame number
	bool cap = true; //Framecap an oder ausschalten

    //Timer zum festlegen der FPS
	Timer fps;
    //Timer zum errechnen der weltweit vergangenen Zeit
	Timer worldtime;
    worldtime.start();

    //calculate the small time between two frames in ms
    int oldTime = 0;
    int newTime = 0;
    int dt = 1;

    //initialize random generator
    seed(time(NULL));

	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Setup our window and renderer, this time let's put our window in the center
	//of the screen
	SDL_Window *window = SDL_CreateWindow("Newton", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

    //generate new objects
    std::vector<Star> stars;
    int nstars = NSTARS;
    for (int i = 0; i < nstars; i++) {
      stars.push_back(Star(frand(0,SCREEN_WIDTH),frand(0,SCREEN_HEIGHT),frand(-1,1),frand(-1,1),frand(1,2)));
    }

    //collision detection
    Collision coll;

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

	//our event structure
	SDL_Event e;

	while (!quit){
        //start the fps timer
        fps.start();

		//Read any events that occured, for now we'll just quit if any event occurs
		while (SDL_PollEvent(&e)){
			//If user closes the window
			if (e.type == SDL_QUIT){
				quit = true;
			}
			//If user presses any key
			if (e.type == SDL_KEYDOWN){
                //do something
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
            mi = pow(stars[i].R(),3)*rho;

            float s1 = 0., sx2 = 0., sy2 = 0.;
            for (int j = 0; j < nstars; j++) {
                if(i != j) {
                    float xj, yj;
                    xj = stars[j].x();
                    yj = stars[j].y();

                    if(xi != xj and yi != yj) {
                        float rij, mj;

                        mj = pow(stars[j].R(),3)*rho;
                        rij = sqrt(pow(xi-xj,2.)+pow(yi-yj,2.));
                        s1 += -gamma*mi*mj/pow(rij,3.);
                        sx2 += +gamma*mi*mj*xj/pow(rij,3.);
                        sy2 += +gamma*mi*mj*yj/pow(rij,3.);
                        collision = false;
                    } else {
                        collision = true;
                    }
                }
            }

            if(!collision) {
                // Runge-Kutta 4. Ordnung
                // löst lineare DGL der Form x'' = ww*x + bet*x' + al.
                float wwx = s1/mi, betx = 0., alx = sx2/mi;
                float k1[4], k2[4];
                float vx = vxi;
                float x = xi;
                k1[0] = vx;
                k2[0] = wwx*x + betx*vx + alx;
                k1[1] = vx + 0.5*k2[0]*dt;
                k2[1] = wwx*(x + 0.5*k1[0]*dt) + betx*(vx + 0.5*k2[0]*dt) + alx; //rho*(t+0.5*dt)+
                k1[2] = vx + 0.5*k2[1]*dt;
                k2[2] = wwx*(x + 0.5*k1[1]*dt) + betx*(vx + 0.5*k2[1]*dt) + alx; //rho*(t+0.5*dt)+
                k1[3] = vx + k2[2]*dt;
                k2[3] = wwx*(x + k1[2]*dt) + betx*(vx + k2[2]*dt) + alx; //rho*(t+dt)+
                x  += dt*(1./6*k1[0] + 1./3*k1[1] + 1./3*k1[2] + 1./6*k1[3]);
                vx += dt*(1./6*k2[0] + 1./3*k2[1] + 1./3*k2[2] + 1./6*k2[3]);
                //ind->ax = ww*x+bet*vx+alx;

                float wwy = s1/mi, bety = 0., aly = sy2/mi;
                float vy = vyi;
                float y = yi;
                k1[0] = vy;
                k2[0] = wwy*y + bety*vy + aly;
                k1[1] = vy + 0.5*k2[0]*dt;
                k2[1] = wwy*(y + 0.5*k1[0]*dt) + bety*(vy + 0.5*k2[0]*dt) + aly; //rho*(t+0.5*dt)+
                k1[2] = vy + 0.5*k2[1]*dt;
                k2[2] = wwy*(y + 0.5*k1[1]*dt) + bety*(vy + 0.5*k2[1]*dt) + aly; //rho*(t+0.5*dt)+
                k1[3] = vy + k2[2]*dt;
                k2[3] = wwy*(y + k1[2]*dt) + bety*(vy + k2[2]*dt) + aly; //rho*(t+dt)+
                y  += dt*(1./6*k1[0] + 1./3*k1[1] + 1./3*k1[2] + 1./6*k1[3]);
                vy += dt*(1./6*k2[0] + 1./3*k2[1] + 1./3*k2[2] + 1./6*k2[3]);
                //ay = ww*y+bet*vy+aly;

                vxi = vx;
                vyi = vy;
                xi = x;
                yi = y;
                stars[i].set_v(vxi,vyi);
                stars[i].set_pos(xi,yi);
            }

            //collision detection
            for (int j = i+1; j < nstars; j++) {
                bool collision = coll.check_collision(&stars[i],&stars[j]);
                //std::cout << collision << std::endl;
                if(collision) {
                    float xj = stars[j].x();
                    float yj = stars[j].y();
                    float vxj = stars[j].vx();
                    float vyj = stars[j].vy();
                    float mj = pow(stars[j].R(),3.)*rho;
                    float vrel = pow(pow(vxi-vxj,2.)+pow(vyi-vyj,2.),.5);
                    //std::cout << "vrel = " << vrel << std::endl;
                    //momentum conservation
                    if(vrel <= vcrit) {
                        xi = (xi*mi+xj*mj)/(mi+mj);
                        yi = (yi*mi+yj*mj)/(mi+mj);
                        vxi = (mi*vxi + mj*vxj)/(mi+mj);
                        vyi = (mi*vyi + mj*vyj)/(mi+mj);
                        mi = mi + mj;
                        stars[i].set_pos(xi,yi);
                        stars[i].set_v(vxi,vyi);
                        float newR = pow(pow(stars[i].R(),3.) + pow(stars[j].R(),3.),1/3.);
                        //std::cout <<stars[i].R()<<" "<<stars[j].R()<<" "<< newR << std::endl;
                        stars[i].set_R(newR);
                        stars.erase(stars.begin() + j);
                        nstars--;
                    }
                    collision = false;
                }
            }

            //mirrored boundaries
            if(xi<=0) stars[i].set_pos(SCREEN_WIDTH,yi);
            if(xi>=SCREEN_WIDTH) stars[i].set_pos(0,yi);
            if(yi<=0) stars[i].set_pos(xi,SCREEN_HEIGHT);
            if(yi>=SCREEN_HEIGHT) stars[i].set_pos(xi,0);
        }

        //check total mass
        mtot_save = mtot;
        mtot = 0;
        for (int i = 0; i < nstars; i++) {
            mtot += pow(stars[i].R(),3)*rho;
        }

        if(mtot_save != 0.0 and fabs(mtot_save-mtot)/mtot >= 0.001 ) {
            std::cout << "Mass not conserved. Bug?" << std::endl;
            std::cout << "mtot_save = " << mtot_save << ", mtot = " << mtot << std::endl;
            exit(0);
        }

        //Rendering
		SDL_RenderClear(renderer);
		//Draw the background white
        boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 255, 255, 255, 255);

        //draw the surface of the moon
        for (int i = 0; i < nstars; i++) {
            //stars[i].update_position(dt/50);
            drawStar(&stars[i], renderer);
        }

        //render
		SDL_RenderPresent(renderer);

        // Timer related stuff
        oldTime = newTime;
        newTime = worldtime.getTicks();
        if (newTime > oldTime) {
            dt = (newTime - oldTime)/1000.; // small time between two frames in s
        }
        if(dt == 0) dt = 1;

        //increment the frame number
        frame++;
        //apply the fps cap
		if( (cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND) ) {
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.getTicks() );
		}

        //update the window caption
		if( worldtime.getTicks() > 1000 ) {
			std::stringstream caption;
			caption << "Newton, FPS = " << 1000.f*frame/worldtime.getTicks() << ", nstars = " << nstars;
            SDL_SetWindowTitle(window,caption.str().c_str());
			worldtime.start();
            frame = 0;
		}
	}

	//Destroy the various items
	cleanup(renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
