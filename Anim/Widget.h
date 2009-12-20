#ifndef __WIDGET__
#define __WIDGET__

#include <SDL.h>
#include "Drawer.h"
#include "Loggable.h"
#include "Matrix.h"
#include "Rect.h"

class Widget : public Loggable {
protected:
   Rect<double> mRect;
   Matrix mTransform;
   std::vector<Widget *> mChildren;
   Widget *mParent;

public:
   Widget(const char *name, Rect<double> rect, Widget *parent = NULL) : Loggable(name), mRect(rect), mTransform(), mChildren(), mParent(parent) {
	  mTransform.SetScale(Vector(mRect.w, -mRect.h));
	  mTransform.SetTranslate(Vector(mRect.x, mRect.y+mRect.h));
     if (NULL != mParent)
     {
        mTransform = mParent->mTransform.Multiply(mTransform);
        mParent->AddChild(this);
     }

     // Temporary
	  mTransform.Print();
   }

   const Rect<double>& GetRect() {
      return mRect;
   }

   void AddChild(Widget *child) {
      mChildren.push_back(child);
   }


   void SetRect(const Rect<double> &r) {
      mRect = r;
      OnResize();
      OnMove();
   }

   virtual void Paint(Drawer &drawer) {
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
      Vector vnew;
      Matrix inv = mTransform.Inverse();
      vnew= inv.Multiply(vec); // TODO use stored inverse
      Log("Received click down");
      std::vector<Widget*>::iterator iter;
      for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
         if ((*iter)->GetRect().Contains(vnew))
            (*iter)->OnClickDown(vec);
      }
   }
   virtual void OnClickUp(const Vector &) {
      Log("Received click up");
   }
};
#endif /* End of include guard: __WIDGET__ */
