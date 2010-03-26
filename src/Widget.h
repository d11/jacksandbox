#ifndef __WIDGET__
#define __WIDGET__

#include "Drawer.h"
#include "Loggable.h"
#include "Transform.h"
#include "Rect.h"

#include <SDL.h>
#include <vector>

using namespace std;
class Widget : public Loggable {
protected:
   Rectangle mRect;
   Transform mTransform;
   vector<Widget *> mChildren;
   Widget *mParent;

public:
   Widget(const char *name, const Rectangle &rect, Widget *parent = NULL) : Loggable(name), mRect(rect), mTransform(rect), mChildren(), mParent(parent) {
     SetRect(rect);
      if (NULL != mParent)
      {
         parent->AddChild(this);
      }
   }

   virtual ~Widget()
   {
      // delete children
      std::vector<Widget*>::iterator iter;
      for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
         delete *iter;
      }
   }

   const Rectangle& GetRect() {
      return mRect;
   }

   void AddChild(Widget *child) {
      mChildren.push_back(child);
   }


   void SetRect(const Rectangle &r) {
      Log("setting rect");
      r.Print();

      mRect = r;
      mTransform = Transform(mRect);
      if (NULL != mParent)
      {
         mTransform = mParent->mTransform.Compose(mTransform);
      }
      Log( mTransform.Print());
      OnResize();
      OnMove();
   }

   virtual void Paint(Drawer &drawer) {
  //  Log("Painting");
      std::vector<Widget*>::iterator iter;
      for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
         (*iter)->Paint(drawer);
      }

   }
   virtual void OnResize() {
      Log("Resized...");
   }
   virtual void OnMove() {
      Log("Moved..");
   }

   virtual void OnClickDown(const Vector &vec) {
      Log("Received click down");
      Vector vnew = mTransform.ApplyInverse(vec);
      std::vector<Widget*>::iterator iter;
      for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
         if ((*iter)->GetRect().Contains(vnew))
            (*iter)->OnClickDown(vec);
      }
   }
   virtual void OnClickUp(const Vector &) {
      Log("Received click up");
   }

   void PrintChildren() {
      Log("Children: {{");
      std::vector<Widget*>::iterator iter;
      for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
         (*iter)->PrintChildren();
      }
      Log("}}");

   }
};
#endif /* End of include guard: __WIDGET__ */
