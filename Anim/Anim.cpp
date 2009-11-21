// Anim.cpp : Defines the entry point for the application.
//

#define WIDTH 640
#define HEIGHT 480

#include <iostream>
#include <cstdlib>
#include <SDL.h>

#include "Anim.h"
#include "Drawer.h"

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
	Drawer drawer(*drawSurface);
   drawer.BeginDrawing();
   Vector v1(10.0, 10.0, 0);
   Vector v2(90.0, 90.0, 0);
   drawer.DrawLine(v1, v2);
   drawer.DrawText(v2, "Yeaaaaah");
   drawer.EndDrawing();

   SDL_UnlockSurface(drawSurface);

   // Copy to the video memory
   if (0 != SDL_BlitSurface(drawSurface, NULL, videoSurface, NULL))
      sdl_fail();

   if (0 != SDL_Flip(videoSurface))
      sdl_fail();

	SDL_Delay(1000);

	finish();
	return 0;
}
