#include "Spline.h"
#include "SplineListener.h"
#include <vector>
#include <list>

using namespace std;

SplineNode::SplineNode(Spline &spline, const Vector &pos) : mPos(pos), mSpline(spline) {}
const Vector &SplineNode::GetPos() const { return mPos; }
void SplineNode::SetPos(const Vector &pos) {
   mPos = pos;
   mSpline.OnNodeMoved(this); // Tell spline we've moved
   NotifyListeners();
}
void SplineNode::Accept(SplineNodeVisitor &visitor) {
   visitor.Visit(*this);
}
Spline::Spline() {}
Spline::~Spline(){
   // Delete all nodes
   tNodes::iterator iter;
   for (iter = mNodes.begin(); iter != mNodes.end(); ++iter) {
      delete *iter;
   }
}
void SplineNode::AddListener(SplineNodeListener *listener) {
   mListeners.push_back(listener);
}

void SplineNode::NotifyListeners() {
   tListeners::iterator iter;
   for (iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
      (*iter)->OnSplineNodeChanged();
   }
}

void Spline::VisitNodes(SplineNodeVisitor &visitor) {
   tNodes::iterator iter;
   for (iter = mNodes.begin(); iter != mNodes.end(); ++iter) {
      (*iter)->Accept(visitor);
   }
}

void Spline::OnNodeMoved(SplineNode *) {
   cout << "Node Moved..." << endl;
}

void Spline::AddNode(const Vector &node){

   // TODO check [0..1] ?
   mNodes.push_back(new SplineNode(*this, node));
   NotifyListeners();
}
double Spline::GetValue(double x) const {
   //if (x < 0.0 || x > 1.0) {cout << "out of range: "<< x << endl; return 0.0;}
   // search for relevant nodes..
   // TODO binarysearch
   tNodes::const_iterator iter = mNodes.begin();
   const Vector *v1 = NULL;

   //TODO check empty
   const Vector *v2 = &((*iter)->GetPos()); ++iter;
   while (iter != mNodes.end()){
      v1 = v2;
      v2 = &((*iter)->GetPos());
      if (v2->GetX() > x) break;
      ++iter;
   }

   //if (!v2) return 0.0; // throw

   // piecewise step
   //return v2->GetY();

   // linear (TODO optimise)
   /*
      double t = (x-v1->GetX())/(v2->GetX()-v1->GetX());
      cout << "t:" << t << endl;
      return t*v2->GetY()+(1.0-t)*v1->GetY();
      */

   //  cubic with 0 derivative @ endpoints (TODO optimise)
   double t = (x-v1->GetX())/(v2->GetX()-v1->GetX());
   double a = 2*(v1->GetY() - v2->GetY()) ;
   double b = 3*(v2->GetY() - v1->GetY());
   double d = v1->GetY();
   return a*t*t*t+b*t*t+d;

}

void Spline::AddListener(SplineListener *listener) {
   mListeners.push_back(listener);
}

void Spline::NotifyListeners() {
   tListeners::iterator iter;
   for (iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
      (*iter)->OnSplineChanged();
   }
}
