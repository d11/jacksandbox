#include "Drawer.h"
#include "Widget.h"
#include "TextBox.h"

#include <sstream>

TextBox::TextBox(const Rectangle &rect, Widget *parent)
   : Widget("TextBox", rect, parent), mString("[Type here]")
{}

void TextBox::Paint(Drawer &drawer)
{
   Vector v1(0.0,0.0);
   Vector v2(1.0,1.0);
   Vector v5(0.0,0.7);

   v1 = mTransform.Apply(v1);
   v2 = mTransform.Apply(v2);
   v5 = mTransform.Apply(v5);
   drawer.DrawRect(v1, v2);
   drawer.DrawText(v5, mString.c_str());
}

void TextBox::OnClickDown(const Vector &vec)
{ }
void TextBox::OnClickUp(const Vector &vec)
{ }
void TextBox::OnMouseMove(const Vector &rel, const Vector &vec)
{ }
void TextBox::OnKeyDown(char key)
{
//TODO
   mString.push_back(key);
}

std::string TextBox::GetText() const
{ return mString; }
void TextBox::SetText(const std::string &s)
{
   mString = s;
}

