#include <SDL.h>
#include "Vector.h"
#include <cairo.h>

class Drawer {
private:
	SDL_Surface *mSDLSurface;
   cairo_surface_t *mCairoSurface;
   cairo_t *mCairo;
public:
	Drawer(SDL_Surface &surf) : mSDLSurface(&surf) { }
   void BeginDrawing();
   void EndDrawing();

	void DrawLine(const Vector &a, const Vector &b);
   void DrawText(const Vector &pos, const char *text);
};
