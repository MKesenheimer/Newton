#include <math.h>
#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "SDL2_own.h"
#include "Functions.h"

int curlRGBA(SDL_Renderer * renderer, const Sint16 x1, const Sint16 y1, const Sint16 x2, const Sint16 y2,
             const Sint16 amp,const Sint16 num_half_waves, int steps, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

    //the input values are short ints, but actually the calculation
    //is made with floats
    float x1f = (float)x1;
    float x2f = (float)x2;
    float y1f = (float)y1;
    float y2f = (float)y2;
    float xf = 0.0f;
    float yf = 0.0f;
    float ampf = (float)amp;
    float theta = 0.0f;

    //constants
    const int npoints = 12;
    const float SQRT2 = 1.41421356f;
    const float Y1 = (2.0f*SQRT2)/7.0f - 1.0f/7.0f;
    const float Y2 = (4.0f*SQRT2)/7.0f - 2.0f/7.0f;
    const float Y3 = SQRT2/2.0f;
    const float Y4 = (3.0f*SQRT2)/7.0f + 2.0f/7.0f;
    const float PI = 3.1415926535897f;
    const float XD = 0.261799388f;
    const float width = sqrt(pow((x1f-x2f),2) + pow((y1f-y2f),2));
    const float xmu1 = width / (num_half_waves * PI);
    const float xd1 = XD * xmu1;

    theta = getAngle(x2f-x1f, y2f-y1f);

    float cosQ = cos(theta);
    float sinQ = sin(theta);


    short s[npoints*num_half_waves];
    short t[npoints*num_half_waves];

    for (int i=0; i<num_half_waves; i++) {
        int j=0;
        s[i*npoints+j] = (short)(xf+1.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y1); j++;
        s[i*npoints+j] = (short)(xf+2.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y2); j++;
        s[i*npoints+j] = (short)(xf+3.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y3); j++;
        s[i*npoints+j] = (short)(xf+4.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y4); j++;
        s[i*npoints+j] = (short)(xf+5.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
        s[i*npoints+j] = (short)(xf+6.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
        s[i*npoints+j] = (short)(xf+7.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
        s[i*npoints+j] = (short)(xf+8.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
        s[i*npoints+j] = (short)(xf+9.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y4); j++;
        s[i*npoints+j] = (short)(xf+10.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y3); j++;
        s[i*npoints+j] = (short)(xf+11.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y2); j++;
        s[i*npoints+j] = (short)(xf+12.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y1); j++;

        xf  += (width / num_half_waves);
        ampf = - ampf;
    }

    short ss[npoints*num_half_waves];
    short ts[npoints*num_half_waves];

    for (int i=0; i<num_half_waves; i++) {
        for (int j=0; j<npoints; j++) {
            ss[i*npoints+j] = cosQ*s[i*npoints+j] - sinQ*t[i*npoints+j] + x1;
            ts[i*npoints+j] = sinQ*s[i*npoints+j] + cosQ*t[i*npoints+j] + y1;
        }
    }

    return bezierRGBA(renderer, ss, ts, npoints*num_half_waves, steps, r, g, b, a);
}
