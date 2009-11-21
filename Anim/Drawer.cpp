#include "Drawer.h"

#include <cairo.h>
#include <iostream>
#include <exception>

class Vector;
using namespace std;

void Drawer::BeginDrawing() {
   mCairoSurface = cairo_image_surface_create_for_data ((unsigned char*)mSDLSurface->pixels,
         CAIRO_FORMAT_RGB24,
         mSDLSurface->w,
         mSDLSurface->h,
         mSDLSurface->pitch);
   mCairo = cairo_create(mCairoSurface);
}

void Drawer::EndDrawing() {
   cairo_destroy (mCairo);
   cairo_surface_destroy (mCairoSurface);
}

void Drawer::DrawLine(const Vector &a, const Vector &b) {
   cout << "Drawing line" << endl;
   cairo_new_path(mCairo);
   cairo_set_line_width (mCairo, 1.2);

   cairo_set_source_rgb(mCairo, 1.0, 1.0, 1.0);
   cairo_move_to(mCairo, a.GetX(), a.GetY());
   cairo_line_to(mCairo, b.GetX(), b.GetY());
   cairo_stroke(mCairo);
}

void Drawer::DrawText(const Vector &pos, const char *text) {
   cout << "Drawing text" << endl;
   cairo_select_font_face (mCairo, "terminus", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size (mCairo, 16.0);
   cairo_set_source_rgb (mCairo, 1.0, 1.0, 1.0);
   cairo_move_to (mCairo, pos.GetX(), pos.GetY());
   cairo_show_text (mCairo, text);
}