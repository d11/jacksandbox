#ifndef __DRAWER__
#define __DRAWER__

#include <SDL.h>
#include "Vector.h"
#include <cairo/cairo.h>
class Drawer {
public:
   virtual void BeginDrawing() = 0;
   virtual void EndDrawing() = 0;
	virtual void DrawLine(const Vector &a, const Vector &b) = 0;
   virtual void DrawText(const Vector &pos, const char *text) = 0;
};

class CairoDrawer : public Drawer {
private:
	SDL_Surface *mSDLSurface;
   cairo_surface_t *mCairoSurface;
   cairo_t *mCairo;
public:
	CairoDrawer(SDL_Surface &surf) : mSDLSurface(&surf) { }
   void BeginDrawing();
   void EndDrawing();

	void DrawLine(const Vector &a, const Vector &b);
   void DrawText(const Vector &pos, const char *text);
};

class NullDrawer : public Drawer {
   void BeginDrawing() {};
   void EndDrawing() {};

	void DrawLine(const Vector &a, const Vector &b) {};
   void DrawText(const Vector &pos, const char *text) {};
};

#endif /* End of include guard: __DRAWER__ */
