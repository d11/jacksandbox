#include <vector>
#include "Vector.h"
#include "Spline.h"

class SplineNodeWiggler : public SplineNodeVisitor
{
private:
   vector<double> mFreqs;
   double mT;
   vector<double>::iterator mIter;
public:
   SplineNodeWiggler() : mT(0.0) {}
   void SetFreqs(const vector<double> &freqs) { mFreqs = freqs; mIter = mFreqs.begin(); }
   void SetTime(double t) { mT = t; }

   void Visit(SplineNode &node) {
      if (mFreqs.empty()) return;
      Vector v = node.GetPos();
      double x = v.GetX();
      double y = 0.5 + 0.5*sin(*mIter*2.0*M_PI*mT);
      mIter++; if (mIter == mFreqs.end()) mIter = mFreqs.begin();
      node.SetPos(Vector(x, y));
   }
};
