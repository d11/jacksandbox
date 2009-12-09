#include <list>

using namespace std;

class Bone {
private:
   list<Bone> mChildren;
   double mLength;
   double mAngle;

   void UpdateAbsoluteCoordinate() { }

public:
   Bone() {}
   void AddChild(Bone &node) { mChildren.push_back(node);  }
   void Rotate(double angle);
};

class Skeleton {
private:
   Bone mRoot;
public:
   Skeleton() : mRoot() {}
};
