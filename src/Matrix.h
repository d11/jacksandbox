#ifndef __MATRIX__
#define __MATRIX__

#include <sstream>
#include <string>
#include "Vector.h"

class Matrix {
private:
   double mEntries[3][3];
public:
   Matrix() {
      SetIdentity();
   }

   Matrix(const Matrix &m) {
      for (int i = 0; i <= 2; ++i) {
         for (int j = 0; j <= 2; ++j) {
            mEntries[i][j] = m.mEntries[i][j];
         }
      }
   }

   // TODO do properly (with 'swap')
   Matrix &operator=(const Matrix &other) {
      for (int i = 0; i <= 2; ++i) {
         for (int j = 0; j <= 2; ++j) {
            mEntries[i][j] = other.mEntries[i][j];
         }
      }
      return *this;
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

   void SetZero() {
      for (int i = 0; i <= 2; ++i) {
         for (int j = 0; j <= 2; ++j) {
            mEntries[i][j] = 0.0;
         }
      }
   }

   void SetScale(Vector s) {
      mEntries[0][0] = s.GetX();
      mEntries[0][1] = 0.0;
      mEntries[1][0] = 0.0;
      mEntries[1][1] = s.GetY();
   }

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

   string Print() const {
      ostringstream s;
      s << endl;
      for (int i = 0; i <= 2; ++i) {
         s << "  [";
         for (int j = 0; j <= 2; ++j) {
            s << mEntries[i][j] << "\t";
         }
         s << "]" << endl;
      }

      return s.str();
   }

   // ROW OP'S - IN PLACE

   void RowAddMult(int row, const Vector &v, double mul){
      mEntries[row][0] += mul*v.GetX();
      mEntries[row][1] += mul*v.GetY();
      mEntries[row][2] += mul*v.GetW();
   }

   void ScaleRow(int row, double mul) {
      mEntries[row][0] *= mul;
      mEntries[row][1] *= mul;
      mEntries[row][2] *= mul;
   }

   Vector GetRow(int row) const
   {
      return Vector(mEntries[row][0], mEntries[row][1], mEntries[row][2]);
   }

   // Find inverse by gaussian reduction
   Matrix Inverse() const {
      Matrix m(*this);
      Matrix inv; // identity
      double pivot = m.mEntries[0][0];
      if (pivot == 0)
      { // do something
         cout << "OH NO!" << endl;
      }
      inv.ScaleRow(0, 1.0/pivot);
      inv.RowAddMult(1, GetRow(0), -m.mEntries[1][0]);
      inv.RowAddMult(2, GetRow(0), -m.mEntries[2][0]);
      m.ScaleRow(0, 1.0/pivot);
      m.RowAddMult(1, GetRow(0), -m.mEntries[1][0]);
      m.RowAddMult(2, GetRow(0), -m.mEntries[2][0]);

      pivot = m.mEntries[1][1];
      if (pivot == 0)
      { // do something
         cout << "OH NO!" << endl;
      }
      inv.ScaleRow(1, 1.0/pivot);
      inv.RowAddMult(0, GetRow(1), -m.mEntries[0][1]);
      inv.RowAddMult(2, GetRow(1), -m.mEntries[2][1]);

      m.ScaleRow(1, 1.0/pivot);
      m.RowAddMult(0, GetRow(1), -m.mEntries[0][1]);
      m.RowAddMult(2, GetRow(1), -m.mEntries[2][1]);

      pivot = m.mEntries[2][2];
      if (pivot == 0){
         // do something
         cout << "OH NO!" << endl;
      }
      inv.ScaleRow(2, 1.0/pivot);
      inv.RowAddMult(0, GetRow(2), -m.mEntries[0][2]);
      inv.RowAddMult(1, GetRow(2), -m.mEntries[1][2]);

      /*
      // Check
      Matrix id_hopefully;
      id_hopefully = Multiply(inv);
      id_hopefully.Print();
      */

      return inv;
   }

};

#endif /* End of include guard: __MATRIX__ */
