#include "Drawer.h"
#include "Widget.h"
#include "Slider.h"

#include <sstream>

Slider::Slider(const Rectangle &rect, Widget *parent)
   : Widget("Slider", rect, parent), mValue(0.5)
{ }

void Slider::Paint(Drawer &drawer) {
   Vector v1(0.0,0.0);
   Vector v2(1.0,1.0);
   Vector v3(mValue, 0.0);
   Vector v4(mValue,1.0);
   Vector v5(0.0,0.7);

   v1 = mTransform.Apply(v1);
   v2 = mTransform.Apply(v2);
   v3 = mTransform.Apply(v3);
   v4 = mTransform.Apply(v4);
   v5 = mTransform.Apply(v5);
   drawer.DrawRect(v1, v2);
   drawer.DrawLine(v3, v4);
   std::ostringstream s;
   s << mValue;
   drawer.DrawText(v5, s.str().c_str());
}
void Slider::OnClickDown(const Vector &vec)
{
   mValue = mTransform.ApplyInverse(vec).GetX();
}
void Slider::OnClickUp(const Vector &vec)
{
}
void Slider::OnMouseMove(const Vector &rel, const Vector &vec)
{
}

double Slider::GetValue() const
{
   return mValue;
}
void Slider::SetValue(double v)
{
   mValue = v;
}
