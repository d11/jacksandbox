#ifndef __SPLINE__
#define __SPLINE__

#include "SplineListener.h"
#include <vector>
#include <list>

//using namespace std;


class Spline {
private:
	typedef std::vector<Vector*> tNodes;
	tNodes mNodes;
   typedef std::list<SplineListener*> tListeners;
   tListeners mListeners;
public:
	Spline() {}
	~Spline(){
		// Delete all nodes
		tNodes::iterator iter;
		for (iter = mNodes.begin(); iter != mNodes.end(); ++iter) {
			delete *iter;
		}
	}

	void AddNode(const Vector &node){

		// TODO check [0..1] ?
		mNodes.push_back(new Vector(node));
      NotifyListeners();
	}
	double GetValue(double x) const {
		if (x < 0.0 || x > 1.0) {cout << "out of range: "<< x << endl; return 0.0;}
		// search for relevant nodes..
		// TODO binarysearch
		tNodes::const_iterator iter = mNodes.begin();
		Vector *v1 = NULL; 
		Vector *v2 = *iter; ++iter;
		while (iter != mNodes.end()){
			v1 = v2;
			v2 = *iter;
			if (v2->GetX() > x) break; 
			++iter;
		}

		if (!v2) return 0.0; // throw

		// piecewise step
		/*
		return v2->GetY();
		*/

		// linear (TODO optimise)
		double t = (x-v1->GetX())/(v2->GetX()-v1->GetX());
		cout << "t:" << t << endl;
		return t*v1->GetY()+(1.0-t)*v2->GetY();

		// TODO cubic / cos?
	}

   void AddListener(SplineListener *listener) {
      mListeners.push_back(listener);
   }

   void NotifyListeners() {
      tListeners::iterator iter;
      for (iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
         (*iter)->OnSplineChanged();
      }
   }
};
#endif /* End of include guard: __SPLINE__ */
