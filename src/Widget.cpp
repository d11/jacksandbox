#include "Widget.h"
#include "Drawer.h"
#include "Loggable.h"
#include "Transform.h"
#include "Rect.h"
#include <vector>
using namespace std;

Widget::Widget(const char *name, const Rectangle &rect, Widget *parent)
: Loggable(name), mRect(rect), mTransform(rect), mParent(parent)
{
   SetRect(rect);
   if (NULL != mParent)
   {
      parent->AddChild(this);
   }
}

Widget::~Widget() {
   // delete children - TODO sort this out
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      delete *iter;
   }
}

const Rectangle &Widget::GetRect() {
   return mRect;
}

void Widget::AddChild(Widget *child) {
   mChildren.push_back(child);
}

void Widget::SetRect(const Rectangle &r) {
   Log("setting rect");

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

void Widget::Paint(Drawer &drawer){
   //Log("Painting");
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      (*iter)->Paint(drawer);
   }

}
void Widget::OnResize() {
   Log("Resized...");
}
void Widget::OnMove() {
   Log("Moved..");
}

// TODO make all this neater...

void Widget::OnClickDown(const Vector &vec) {
   Log("Received click down");
   Vector vnew = mTransform.ApplyInverse(vec);
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      if ((*iter)->GetRect().Contains(vnew))
         (*iter)->OnClickDown(vec);
   }
}

void Widget::OnClickUp(const Vector &vec) {
   Log("Received click up");
   Vector vnew = mTransform.ApplyInverse(vec);
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      if ((*iter)->GetRect().Contains(vnew))
         (*iter)->OnClickUp(vec);
   }
}

void Widget::OnMouseMove(const Vector &rel, const Vector &vec) 
{
   Log("Recieved mouse move");
   Vector vnew = mTransform.ApplyInverse(vec);
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      if ((*iter)->GetRect().Contains(vnew))
         (*iter)->OnMouseMove(rel,vec);
   }
}

void Widget::PrintChildren() {
   Log("Children: {{");
   vector<Widget*>::iterator iter;
   for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
      (*iter)->PrintChildren();
   }
   Log("}}");
}

const Transform &Widget::GetTransform() const {
   return mTransform;
}
