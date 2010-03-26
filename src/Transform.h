#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "Rect.h"
#include "Matrix.h"

#include <iostream>
// A 2D transformation combining a scale and a translation
// Stores the inverse to avoid recalculating it

class Transform
{
private:
   Matrix mMatrix;
   Matrix mInverse;
public:
   Transform() {}; // Identity transformation

   // Transformation given by an existing matrix
   Transform(const Matrix &matrix)
      : mMatrix(matrix), mInverse(matrix.Inverse())
   {}

   // Transformation sending unit square to the given rect
   Transform(const Rectangle &rect) {
      mMatrix.SetScale(Vector(rect.w, -rect.h));
      mMatrix.SetTranslate(Vector(rect.x, rect.y+rect.h));
      mInverse.SetScale(Vector(1.0/rect.w, -1.0/rect.h));
      mInverse.SetTranslate(Vector(-rect.x/rect.w, (rect.y+rect.h)/rect.h));
   }

   Transform Compose(const Transform &other) const  {
      Transform t(*this);
      t.mMatrix = t.mMatrix.Multiply(other.mMatrix);
      t.mInverse = other.mInverse.Multiply(t.mInverse);
      return t;
   }

   Vector Apply(const Vector &v) const {
      return mMatrix.Multiply(v);
   }

   Vector ApplyInverse(const Vector &v) const {
      return mInverse.Multiply(v);
   }

   Transform Inverse() const {
      Transform t;
      t.mMatrix = mInverse;
      t.mInverse = mMatrix;
      return t;
   }

   Transform &operator=(const Transform &other) { // TODO do properly
      mMatrix = other.mMatrix;
      mInverse = other.mInverse;
      return *this;
   }

   // Return the rectangle produced by applying the transform to the unit square
   Rectangle GetRect() const {
      Rectangle r;
      Vector v1(0.0, 0.0);
      Vector v2(1.0, 1.0);
      v1 = Apply(v1);
      v2 = Apply(v2);
      v2 = v2 - v1;
      r.x = v1.GetX();
      r.y = v1.GetY();
      r.w = v2.GetX();
      r.h = v2.GetY();
      return r;
   }

   Matrix GetMatrix() const {
      return mMatrix;
   }

   string Print() const {
      return mMatrix.Print() + "\nInv." + mInverse.Print();
   }
};


#endif /* End of include guard: __TRANSFORM__ */
