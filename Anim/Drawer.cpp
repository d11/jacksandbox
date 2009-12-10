#include "Drawer.h"
#include "Vector.h"

#include <cairo/cairo.h>
#include <iostream>
#include <exception>

using namespace std;

void CairoDrawer::BeginDrawing() {
   mCairoSurface = cairo_image_surface_create_for_data ((unsigned char*)mSDLSurface->pixels,
         CAIRO_FORMAT_RGB24,
         mSDLSurface->w,
         mSDLSurface->h,
         mSDLSurface->pitch);
   mCairo = cairo_create(mCairoSurface);
}

void CairoDrawer::EndDrawing() {
   cairo_destroy (mCairo);
   cairo_surface_destroy (mCairoSurface);
}

void CairoDrawer::DrawLine(const Vector &a, const Vector &b) {
   cairo_new_path(mCairo);
   cairo_set_line_width (mCairo, 1.2);

   cairo_set_source_rgb(mCairo, 1.0, 1.0, 1.0);
   cairo_move_to(mCairo, a.GetX(), a.GetY());
   cairo_line_to(mCairo, b.GetX(), b.GetY());
   cairo_stroke_preserve(mCairo);
}

void CairoDrawer::DrawRect(const Vector &a, const Vector &b) {
   Vector c = a.SetXFrom(b);
   Vector d = a.SetYFrom(b);
   DrawLine(a, c); DrawLine(c, b); DrawLine(b, d); DrawLine(d, a);
}

void CairoDrawer::DrawText(const Vector &pos, const char *text) {
   cairo_select_font_face (mCairo, "terminus", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_select_font_face (mCairo, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (mCairo, 16.0);
   cairo_set_source_rgb (mCairo, 1.0, 1.0, 1.0);
   cairo_move_to (mCairo, pos.GetX(), pos.GetY());
   cairo_show_text (mCairo, text);
}
