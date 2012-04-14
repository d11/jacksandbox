#ifndef __SLIDER__
#define __SLIDER__

#include "Widget.h"

class Slider : public Widget {
private:
   double mValue;
public:
   Slider(const Rectangle &rect, Widget *parent = NULL);
   virtual void Paint(Drawer &drawer);

   virtual void OnClickDown(const Vector &vec);
   virtual void OnClickUp(const Vector &vec);
   virtual void OnMouseMove(const Vector &rel, const Vector &vec);

   double GetValue() const;
   void SetValue(double v);
};

#endif /* End of include guard: __SLIDER__ */
