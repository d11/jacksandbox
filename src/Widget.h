#ifndef __WIDGET__
#define __WIDGET__

#include "Transform.h"
#include "Rect.h"
#include "Loggable.h"
#include <vector>
using namespace std;

class Drawer;

class Widget : public Loggable {
protected:
   Rectangle mRect;
   Transform mTransform;
   vector<Widget *> mChildren;
   Widget *mParent;

public:
   Widget(const char *name, const Rectangle &rect, Widget *parent = NULL);
   virtual ~Widget();

   const Rectangle& GetRect();
   void AddChild(Widget *child);
   void SetRect(const Rectangle &r);
   virtual void Paint(Drawer &drawer);

   virtual void OnResize();
   virtual void OnMove();
   virtual void OnClickDown(const Vector &vec);
   virtual void OnMouseMove(const Vector &rel, const Vector &vec);
   virtual void OnClickUp(const Vector &);

   void PrintChildren();

   const Transform &GetTransform() const;
};
#endif /* End of include guard: __WIDGET__ */
