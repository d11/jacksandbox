#ifndef __SPLINEVIEW__
#define __SPLINEVIEW__

#include "SplineListener.h"
#include "Widget.h"
#include "Spline.h"
#include "Matrix.h"

using namespace std;

class SplineView : public Widget, SplineListener {
private:
   Spline *mSpline;
   double mStepSize;
   Matrix mTransform;
public:
   SplineView(Spline *spline, SDL_Rect rect, Drawer &drawer) 
	   : Widget("SplineView", rect, drawer), 
	     SplineListener(spline), 
		 mSpline(spline), 
		 mStepSize(0.02),
		 mTransform()
   {
      mSpline->AddListener(this);
	  mTransform.SetScale(Vector(mRect.w, -mRect.h));
	  mTransform.SetTranslate(Vector(mRect.x, mRect.y+mRect.h));
	  mTransform.Print();
   }

   virtual void OnSplineChanged() {
   }

   virtual void Paint() {

      Vector c1(0.0, 0.0, 1.0);
      Vector c2(1.0, 1.0, 1.0);
      c1 = mTransform.Multiply(c1);
      c2 = mTransform.Multiply(c2);
      mDrawer.DrawRect(c1,c2);

      if (!mSpline) {cout << "no spline!!" << endl; return;}
      Vector v1(0.0, 0.0);
      Vector v2(0.0, 0.0);
      cout << "painting spline" << endl;
      double x1 = 0.0;
      double x2 = 0.0;
      double y1 = 0.0;
      double y2 = mSpline->GetValue(x2);
      while (x1 <= 1.0 - mStepSize) {

         x1 = x2;
         y1 = y2;

         x2 += mStepSize;
         y2 = mSpline->GetValue(x2);

         v1 = mTransform.Multiply(Vector(x1, y1, 1.0));
         v2 = mTransform.Multiply(Vector(x2, y2, 1.0));
         mDrawer.DrawLine(v1, v2);

      }
   }
};
#endif /* End of include guard: __SPLINEVIEW__ */
