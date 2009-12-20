#ifndef __SPLINEVIEW__
#define __SPLINEVIEW__

#include "SplineListener.h"
#include "Widget.h"
#include "Spline.h"
#include "Matrix.h"

using namespace std;

class NodePainter : public SplineNodeVisitor {
private:
   Drawer &mDrawer;
   Matrix &mTransform; // TODO put elsewher
public:
   NodePainter(Drawer &drawer, Matrix &trans) : mDrawer(drawer), mTransform(trans) {

	  mTransform.Print();
   }
   void Visit(SplineNode &node)
   {
      Vector v = node.GetPos();
      v.MakeFree();
      v = mTransform.Multiply(v);

      cout << "visiting " << v.GetX() << ", " << v.GetY() << endl;
      mDrawer.DrawCircle(v, 5.0);
   }
};

class SplineView : public Widget, SplineListener {
private:
   Spline *mSpline;
   double mStepSize;
public:
   SplineView(Spline *spline, Rect<double> rect, Widget *parent = NULL)
	   : Widget("SplineView", rect, parent), SplineListener(spline),
		 mSpline(spline), mStepSize(0.02)
   {
      mSpline->AddListener(this);
   }

   virtual void OnSplineChanged() {
   }

   virtual void Paint(Drawer &drawer) {
      Vector c1(0.0, 0.0, 1.0);
      Vector c2(1.0, 1.0, 1.0);
      c1 = mTransform.Multiply(c1);
      c2 = mTransform.Multiply(c2);
      drawer.DrawRect(c1,c2);

      if (!mSpline) {cout << "no spline!!" << endl; return;}
      Vector v1(0.0, 0.0);
      Vector v2(0.0, 0.0);

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
         drawer.DrawLine(v1, v2);
      }

      NodePainter nodePainter(drawer, mTransform);
      mSpline->VisitNodes(nodePainter);

   }
};
#endif /* End of include guard: __SPLINEVIEW__ */
