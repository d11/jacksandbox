#ifndef __SPLINEVIEW__
#define __SPLINEVIEW__

#include "Rect.h"
#include "SplineListener.h"
#include "Widget.h"
#include "Spline.h"
#include "Matrix.h"

class Drawer;
class Matrix;
class SplineView;

/* OBSOLETE
class NodePainter : public SplineNodeVisitor {
private:
   Drawer &mDrawer;
   Matrix &mTransform; // TODO put elsewher
public:
   NodePainter(Drawer &drawer, Matrix &trans);
   void Visit(SplineNode &node);
};*/


class NodeView : public Widget, SplineNodeListener {
private:
   SplineNode *mNode;
   SplineView *mSplineView;
public:
   NodeView(SplineNode *node, const Rectangle &rect, SplineView *parent = NULL);
   virtual void OnSplineNodeChanged();
   virtual void Paint(Drawer &drawer);
   void OnClickDown(const Vector &vec);
};

class NodeViewMaker : public SplineNodeVisitor {
private:
   SplineView *mParent;
   Matrix mTrans;

public:
   NodeViewMaker(SplineView *parent, const Matrix &trans);
   virtual void Visit(SplineNode &node);
};

class SplineView : public Widget, SplineListener {
private:
   Spline *mSpline;
   double mStepSize;
public:
   SplineView(Spline *spline, const Rectangle &rect, Widget *parent = NULL);
   virtual void OnSplineChanged();
   void UpdateNodeView(NodeView *nv, SplineNode *node);
   virtual void Paint(Drawer &drawer);
};
#endif /* End of include guard: __SPLINEVIEW__ */
