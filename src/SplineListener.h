#ifndef __SPLINELISTENER__
#define __SPLINELISTENER__

class Spline;
class SplineNode;

class SplineListener {
private:
   Spline *mSpline;
public:
   SplineListener(Spline *spline) : mSpline(spline) {}
   virtual void OnSplineChanged() {};
};

class SplineNodeListener {
   private:
      SplineNode *mNode;
   public:
      SplineNodeListener(SplineNode *node) : mNode(node) {}
      virtual void OnSplineNodeChanged() {};
};
#endif /* End of include guard: __SPLINELISTENER__ */
