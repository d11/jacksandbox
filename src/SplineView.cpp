#include "SplineView.h"
#include "SplineListener.h"
#include "Widget.h"
#include "Spline.h"
#include "Matrix.h"
#include "Drawer.h"


using namespace std;

/* OBSOLETE
NodePainter::NodePainter(Drawer &drawer, Matrix &trans)
   : mDrawer(drawer), mTransform(trans) {
   //mTransform.Print();
}
void NodePainter::Visit(SplineNode &node)
{
   Vector v = node.GetPos();
   v.MakeFree();
   v = mTransform.Multiply(v);

   cout << "visiting " << v.GetX() << ", " << v.GetY() << endl;
   mDrawer.DrawCircle(v, 5.0);
}
*/

NodeView::NodeView(SplineNode *node, const Rectangle &rect, SplineView *parent)
         : Widget("SplineNodeView", rect, (Widget*)parent), SplineNodeListener(node),
           mNode(node), mSplineView(parent), mBeingDragged(false)
{
   mNode->AddListener(this);
}
void NodeView::OnSplineNodeChanged() {
   mSplineView->UpdateNodeView(this, mNode);
}
void NodeView::Paint(Drawer &drawer) {
   //Vector v = mNode->GetPos();
   //v.MakeFree();
   //v = mTransform.Multiply(v);
   //cout << "Painting spline node " << v.GetX() << ", " << v.GetY() << endl;
   //cout << "Painting spline node " << endl;

   Vector v(0.5, 0.5);
   v = mTransform.Apply(v);
   drawer.DrawCircle(v, 5.0);

   Vector c1(0.0, 0.0);
   Vector c2(1.0, 1.0);
   c1 = mTransform.Apply(c1);
   c2 = mTransform.Apply(c2);
   drawer.DrawRect(c1,c2);
   Widget::Paint(drawer);
}

// TODO - get dragging working
// Requires: OnMouseMove and OnMouseUp actions
// also want: move general transforming into Widget where possible
// Do not recompute inverse every time
//
// Events system for repainting



void NodeView::OnClickDown(const Vector &vec)
{
   mBeingDragged = true;
   /*
   Vector vec2 = mTransform.ApplyInverse(vec);
   vec2.Print();
   Vector v = mNode->GetPos();
   double y = v.GetY();
   double y2 = (y -0.05+ vec2.GetY())/2.0;
   if (y2 > 1.0) y2 -= 1.0;
   mNode->SetPos(Vector(v.GetX(), y2));
   //mNode->SetPos((v*2.0-vec2)*0.5);
   Widget::OnClickDown(vec);*/
}

void NodeView::OnClickUp(const Vector &)
{
   mBeingDragged = false;
}

void NodeView::OnMouseMove(const Vector &rel, const Vector &vec)
{
   if (mBeingDragged)
   {
      Rectangle r = mRect;
      Vector vnew = mParent->GetTransform().ApplyInverse(vec);

      r.x = vnew.GetX();
      r.y = vnew.GetY();
      //SetRect(r);
      mNode->SetPos(Vector(r.x, r.y));
   }
   Widget::OnMouseMove(rel, vec);
}

NodeViewMaker::NodeViewMaker(SplineView *parent, const Matrix &trans)
: mParent(parent), mTrans(trans)
{ }

void NodeViewMaker::Visit(SplineNode &node)
{
   const double rectSize = 10.0;

   Rect<double> rect;

   Vector v_centre = node.GetPos();

   rect.x = v_centre.GetX()-0.05;
   rect.y = v_centre.GetY()-0.05;
   rect.w = 0.1;
   rect.h = 0.1;
   new NodeView(&node, rect, mParent);
}

SplineView::SplineView(Spline *spline, const Rectangle &rect, Widget *parent)
	   : Widget("SplineView", rect, parent), SplineListener(spline),
		 mSpline(spline), mStepSize(0.05) // TODO use spline elevation for drawing..
   {
      mSpline->AddListener(this);
      NodeViewMaker nodeViewMaker(this, mTransform.GetMatrix());
      mSpline->VisitNodes(nodeViewMaker);
   }

void SplineView::OnSplineChanged() {
   // update nodeview positions
}

void SplineView::UpdateNodeView(NodeView *nv, SplineNode *node)
{
   const double rectSize = 10.0;

   Rect<double> rect;

   Vector v_centre = node->GetPos();

   rect.x = v_centre.GetX()-0.05;
   rect.y = v_centre.GetY()-0.05;
   rect.w = 0.1;
   rect.h = 0.1;

   nv->SetRect(rect);
}

void SplineView::Paint(Drawer &drawer) {
   Vector c1(0.0, 0.0);
   Vector c2(1.0, 1.0);
   c1 = mTransform.Apply(c1);
   c2 = mTransform.Apply(c2);
   drawer.DrawRect(c1,c2);

   if (!mSpline) {cout << "no spline!!" << endl; return;}
   Vector v1(0.0, 0.0);
   Vector v2(0.0, 0.0);

   double x1 = 0.0;
   double x2 = 0.0;
   double y1 = 0.0;
   double y2 = mSpline->GetValue(x2);
   while (x1 < 1.0 - mStepSize) {

      x1 = x2;
      y1 = y2;

      x2 += mStepSize;
      y2 = mSpline->GetValue(x2);

      v1 = mTransform.Apply(Vector(x1, y1, 1.0));
      v2 = mTransform.Apply(Vector(x2, y2, 1.0));
      drawer.DrawLine(v1, v2);
   }

   Widget::Paint(drawer);
   // NodePainter nodePainter(drawer, mTransform);
   // mSpline->VisitNodes(nodePainter);
}

