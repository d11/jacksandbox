#ifndef __SPLINELISTENER__
#define __SPLINELISTENER__

class Spline;

class SplineListener {
private:
   Spline *mSpline;
public:
   SplineListener(Spline *spline) : mSpline(spline) {}
   virtual void OnSplineChanged() {};
};
#endif /* End of include guard: __SPLINELISTENER__ */
