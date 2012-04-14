#ifndef __TEXTBOX__
#define __TEXTBOX__

#include "Widget.h"

class TextBox : public Widget {
private:
   string mString;
public:
   TextBox(const Rectangle &rect, Widget *parent = NULL);
   virtual void Paint(Drawer &drawer);

   virtual void OnClickDown(const Vector &vec);
   virtual void OnClickUp(const Vector &vec);
   virtual void OnMouseMove(const Vector &rel, const Vector &vec);
   virtual void OnKeyDown(char key);

   string GetText() const;
   void SetText(const string &s);
};

#endif
