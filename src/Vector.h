#ifndef __VECTOR__
#define __VECTOR__

#include <math.h>
#include <iostream>
using namespace std;
class Vector {
private:
   double mX, mY, mW;
public:
   Vector() : mX(0.0), mY(0.0), mW(0.0) {}
   Vector(double x, double y, double w) : mX(x), mY(y), mW(w) {}
   Vector(double x, double y) : mX(x), mY(y), mW(1.0) {}

   Vector(const Vector &other)
      :mX(other.mX), mY(other.mY), mW(other.mW) { }

   inline double GetLength() { return sqrt(mX*mX + mY*mY); }

   inline double GetX() const { return mX; }
   inline double GetY() const { return mY; }
   inline double GetW() const { return mW; }

   inline void MakeFree() { mW = 1.0; }

   Vector FlattenX() { Vector v(*this); v.mX = 0; return v;}
   Vector FlattenY() { Vector v(*this); v.mY = 0; return v;}

   Vector operator+(const Vector &other) const {
      Vector v(*this);
      v.mX += other.mX;
      v.mY += other.mY;
      v.mW *= v.mW; // TODO verify that this is a good idea
      return v;
   }
   Vector operator-(const Vector &other) const {
      Vector v(*this);
      v.mX -= other.mX;
      v.mY -= other.mY;
      v.mW *= v.mW; // TODO verify that this is a good idea
      return v;
   }

   // 'scalar multiplication in 2 dimensions...'
   Vector operator*(const double &factor) const {
      Vector v(*this);
      v.mX *= factor;
      v.mY *= factor;
      return v;
   }

   void operator=(const Vector &other) { // TODO redo properly
      mX = other.mX;
      mY = other.mY;
      mW = other.mW;
   }

   Vector SetXFrom(const Vector &other) const {
      Vector v(*this);
      v.mX = other.mX;
      return v;
   }

   Vector SetYFrom(const Vector &other) const {
      Vector v(*this);
      v.mY = other.mY;
      return v;
   }



   void Print() const {
      cout << "Vector(" << GetX() << ", " << GetY() << ", " << GetW() << ")" << endl;
   }
};
#endif /* End of include guard: __VECTOR__ */
