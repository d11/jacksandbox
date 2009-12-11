#ifndef __MATRIX__
#define __MATRIX__

#include "Vector.h"

class Matrix {
private:
   double mEntries[3][3];
public:
	Matrix() {
		SetIdentity();
	}

   // set translation portion to the given vector
   void SetTranslate(Vector t) {
	  mEntries[0][2] = t.GetX();
	  mEntries[1][2] = t.GetY();
	  mEntries[2][2] = t.GetW();
   }

   void SetIdentity() {
	   for (int i = 0; i <= 2; ++i) {
		   for (int j = 0; j <= 2; ++j) {
			   mEntries[i][j] = (i==j) ? 1.0 : 0.0;
		   }
	   }
   }

   void SetScale(Vector s) {
	  mEntries[0][0] = s.GetX();
	  mEntries[0][1] = 0.0;
	  mEntries[1][0] = 0.0;
	  mEntries[1][1] = s.GetY();
   }

   // TODO check/tidy
   Vector Multiply(const Vector &x) const {
      double vi[3];
      vi[0] = x.GetX();
      vi[1] = x.GetY();
      vi[2] = x.GetW();

      double vs[3];
	  vs[0] = 0.0; vs[1] = 0.0; vs[2] = 0.0;
      for (int i = 0; i <= 2 ; ++i) {
         for (int j = 0; j <= 2; ++j) {
            vs[i]+=mEntries[i][j]*vi[j];
         }
      }
      return Vector(vs[0], vs[1], vs[2]);
   }

   Matrix Multiply(const Matrix &other) const {
      ////TODO test
      Matrix m;
      for (int i = 0; i <= 2; ++i) {
         for (int j = 0; j <= 2; ++j) {
            m.mEntries[i][j] = 0.0;
            for (int k = 0; k <= 2; ++k) {
               m.mEntries[i][j] += mEntries[i][k]*other.mEntries[k][j];
            }
         }
      }
      return m;
   }

   void Print() {
	   for (int i = 0; i <= 2; ++i) {
		 cout << "[";
		 for (int j = 0; j <= 2; ++j) {
			cout << mEntries[i][j] << " \t ";
		 }
		 cout << "]" << endl;
	   }
   }

};

#endif /* End of include guard: __MATRIX__ */
