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

	  //mTransform.Print();
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


class NodeView : public Widget, SplineNodeListener {
   private:
      SplineNode *mNode;
   public:
      NodeView(SplineNode *node, Rect<double> rect, Widget *parent = NULL)
         : Widget("SplineNodeView", rect, parent), SplineNodeListener(node),
           mNode(node)
      {
         mNode->AddListener(this);
      }
      virtual void OnSplineNodeChanged() { }
      virtual void Paint(Drawer &drawer) {
         Vector v = mNode->GetPos();
         v.MakeFree();
         v = mTransform.Multiply(v);
         cout << "Painting spline node " << v.GetX() << ", " << v.GetY() << endl;

         drawer.DrawCircle(v, 5.0);
         Vector c1(mRect.x, mRect.y);
         Vector c2(mRect.x+mRect.w, mRect.y+mRect.h);
         c1 = mTransform.Multiply(c1);
         c2 = mTransform.Multiply(c2);
         drawer.DrawRect(c1,c2);
      }

};

class NodeViewMaker : public SplineNodeVisitor {
   private:
      Widget *mParent;
      Matrix &mTrans;

   public:
      NodeViewMaker(Widget *parent, Matrix &trans) : mParent(parent), mTrans(trans)
      { 
      trans.Print();
      }

      virtual void Visit(SplineNode &node)
      {
         const double rectSize = 10.0;
         double r = rectSize / 2.0;

         Rect<double> rect;

         Vector v_centre = node.GetPos();
         v_centre.Print();
         Vector v_corner(r, r);
         v_corner.MakeFree();
         //v_corner = mTrans.Multiply(v_corner);


         rect.x = v_centre.GetX()-0.1;
         rect.y = v_centre.GetY()-0.1;
         rect.w = 0.2;
         rect.h = 0.2;
         rect.Print();
         new NodeView(&node, rect, mParent);
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
      mTransform.Print();
      NodeViewMaker nodeViewMaker(this, mTransform);
      mSpline->VisitNodes(nodeViewMaker);
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

      Widget::Paint(drawer);
     // NodePainter nodePainter(drawer, mTransform);
     // mSpline->VisitNodes(nodePainter);

   }
};
#endif /* End of include guard: __SPLINEVIEW__ */
