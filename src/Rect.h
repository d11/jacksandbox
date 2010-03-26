#ifndef __RECT__
#define __RECT__

#include <iostream>
#include <SDL.h>
#include "Vector.h"
using namespace std;

template <class T>
class Rect {
public:
   T x, y;
   T w, h;

   Rect() : x(), y(), w(), h() {}
   Rect(SDL_Rect r) : x((T)r.x), y((T)r.y), w((T)r.w), h((T)r.h){ }

   bool Contains(const Vector &vec) const {
      /*
      cout << "vec: " << vec.GetX() << ", " << vec.GetY() << endl;
      cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << endl;
      */
      if (vec.GetX() < x) return false;
      if (vec.GetX() > x + w) return false;
      if (vec.GetY() < y) return false;
      if (vec.GetY() > y + h) return false;
      return true;
   }

   void Print() const {
      cout << "Rectangle: [x: " << x << " y: " << y << " w: " << w << " h: " << h << "]"<< endl;
   }
};

typedef Rect<double> Rectangle;
#endif /* End of include guard: __RECT__ */
