#ifndef __SPLINE__
#define __SPLINE__

#include <iostream>
#include <vector>
#include <list>

#include "Vector.h"
using namespace std;

class SplineNode;
class SplineListener;
class SplineNodeListener;
class Spline;

class SplineNodeVisitor {
public:
   virtual void Visit(SplineNode &node) = 0;
};

class SplineNode {
private:
   Vector mPos;
   Spline &mSpline;
   typedef std::list<SplineNodeListener*> tListeners;
   tListeners mListeners;
public:
   SplineNode(Spline &spline, const Vector &pos);
   const Vector &GetPos() const;
   void SetPos(const Vector &pos) ;
   void Accept(SplineNodeVisitor &visitor) ;
   void AddListener(SplineNodeListener *listener);
   void NotifyListeners();
};

class Spline {
private:
	typedef std::vector<SplineNode*> tNodes;
	tNodes mNodes;
   typedef std::list<SplineListener*> tListeners;
   tListeners mListeners;
public:
	Spline();
	~Spline();

   void VisitNodes(SplineNodeVisitor &visitor) ;

   void OnNodeMoved(SplineNode *) ;

	void AddNode(const Vector &node);
	double GetValue(double x) const;
   void AddListener(SplineListener *listener);

   void NotifyListeners();
};
#endif /* End of include guard: __SPLINE__ */
