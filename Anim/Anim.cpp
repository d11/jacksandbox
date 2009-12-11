// Anim.cpp : Defines the entry point for the application.

#define WIDTH 640
#define HEIGHT 480

#include <iostream>
#include <cstdlib>

#include <cmath>
#include <SDL.h>

#ifndef M_PI
	#define M_PI           3.14159265358979323846
#endif

#include "Anim.h"
#include "Drawer.h"
#include "Skeleton.h"
#include "Spline.h"
#include "SplineView.h"

using namespace std;

void finish()
{
	SDL_Quit();
}

void sdl_fail()
{
	cout << "[SDL failure] " << SDL_GetError() << endl;
	finish();
	abort();
}

int main(int argc, char *argv[]) {
	if (0 != SDL_Init(SDL_INIT_VIDEO))
		sdl_fail();

	SDL_Surface *videoSurface;
	videoSurface = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (NULL == videoSurface)
		sdl_fail();

   SDL_Surface *drawSurface;

   drawSurface = SDL_CreateRGBSurface (
         SDL_HWSURFACE, WIDTH, HEIGHT, 32,
         0x00FF0000, /* Rmask */
         0x0000FF00, /* Gmask */
         0x000000FF, /* Bmask */
         0); /* Amask */


   if (0 != SDL_LockSurface(drawSurface))
      sdl_fail();

   // Do some drawing
	CairoDrawer drawer(*drawSurface);

   int i;
   double hGap = 20.0;
   double vHeight = 100.0;
   double vOffset = HEIGHT/2;
   double hOffset = 50.0;
   drawer.BeginDrawing();
   for (i = 0; i < 26; ++i){
      double x= hOffset+hGap*i;
      double y = vOffset + vHeight*sin(i/25.0*M_PI*2);
      Vector v1(x, y, 0);
      Vector v2(x+(13-i)*3, vOffset + vHeight, 0);
      char c = 65 + i;
      drawer.DrawLine(v1, v2);
      drawer.DrawText(v1, &c);
   }
	Spline spl;
	spl.AddNode(Vector(0.0, 0.2, 0.0));
	spl.AddNode(Vector(0.2, 0.8, 0.0));
	spl.AddNode(Vector(0.5, 0.4, 0.0));
	spl.AddNode(Vector(0.8, 0.5, 0.0));
	spl.AddNode(Vector(1.0, 0.7, 0.0));

	SDL_Rect rec;
	rec.h = 160;
	rec.w = 300;
	rec.x = 20;
	rec.y = 20;
	SplineView sv(&spl, rec, drawer);
	sv.Paint();
drawer.EndDrawing();
   SDL_UnlockSurface(drawSurface);

   // Copy to the video memory
   if (0 != SDL_BlitSurface(drawSurface, NULL, videoSurface, NULL))
      sdl_fail();

   if (0 != SDL_Flip(videoSurface))
      sdl_fail();

	SDL_Event e;
    bool done = false;

    while((!done) && (SDL_WaitEvent(&e))) {
        switch(e.type) {
			case SDL_KEYUP:
                // Handle any key presses here.
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					done = true;
				}
                break;

            case SDL_MOUSEBUTTONDOWN:
                break;

            case SDL_QUIT:
                done = true;
                break;
                
            default:
                break;
        }     
    }      

	finish();
	return 0;
}
