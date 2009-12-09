#ifndef __VECTOR__
#define __VECTOR__

#include <math.h>

class Vector {
private:
   double mX, mY, mW;
public:
   Vector() : mX(0.0), mY(0.0), mW(0.0) {}
   Vector(double x, double y, double w) : mX(x), mY(y), mW(w) {}
   Vector(double x, double y) : mX(x), mY(y), mW(0.0) {}

   inline double GetLength() { return sqrt(mX*mX + mY*mY); }

   inline double GetX() const { return mX; }
   inline double GetY() const { return mY; }
   inline double GetW() const { return mW; }

   void operator=(const Vector &other) {
      mX = other.mX;
      mY = other.mY;
      mW = other.mW;
   }
};
#endif /* End of include guard: __VECTOR__ */
