#ifndef __WIDGET__
#define __WIDGET__

#include <SDL.h>
#include "Drawer.h"
#include "Loggable.h"

class Widget : public Loggable {
protected:
   SDL_Rect mRect;
   Drawer &mDrawer;
public:
   Widget(const char *name, SDL_Rect rect, Drawer &drawer) : Loggable(name), mRect(rect), mDrawer(drawer) {
   }

   void SetDrawer(Drawer &drawer) { mDrawer = drawer; }

   void SetRect(const SDL_Rect &r) {
      mRect = r;
      OnResize();
      OnReposition();
   }

   virtual void Paint() = 0;
   virtual void OnResize() {
      Log("Resized...");
   }
   virtual void OnReposition() {
      Log("Moved..");
   }

   virtual void OnClickDown(const Vector &pos) {
      Log("Received click down");
   }
   virtual void OnClickUp(const Vector &pos) {
      Log("Received click up");
   }
};
#endif /* End of include guard: __WIDGET__ */
