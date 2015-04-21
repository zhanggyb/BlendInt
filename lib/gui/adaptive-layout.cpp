/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <blendint/gui/adaptive-adjustment.hpp>
#include <blendint/gui/adaptive-layout.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

AdaptiveLayout::AdaptiveLayout (Orientation orient, int align, int space)
    : AbstractLayout(), orientation_(orient), alignment_(align), space_(space)
{
  set_size(200, 200);
}

AdaptiveLayout::AdaptiveLayout (int width,
                                int height,
                                const Margin& margin,
                                Orientation orient,
                                int align,
                                int space)

    :
      AbstractLayout(width, height, margin),
      orientation_(orient),
      alignment_(align),
      space_(space)
{
}

AdaptiveLayout::~AdaptiveLayout ()
{
}

AbstractWidget* AdaptiveLayout::AddWidget (AbstractWidget* widget)
{
  if (PushBackSubView(widget)) {
    Adjust();
    RequestRedraw();
    return widget;
  }

  return 0;
}

AbstractWidget* AdaptiveLayout::InsertWidget (int index, AbstractWidget* widget)
{
  if (InsertSubView(index, widget)) {
    Adjust();
    RequestRedraw();
    return widget;
  }

  return 0;
}

AbstractWidget* AdaptiveLayout::InsertWidget (int row,
                                              int column,
                                              AbstractWidget* widget)
{
  if (orientation_ == Horizontal) {
    if (row != 0) {
      DBG_PRINT_MSG(
          "Error: %s",
          "LinearLayout contains only 1 row, and the 1st parameter will be ignored");
    }

    if (InsertSubView(column, widget)) {
      Adjust();
      RequestRedraw();
      return widget;
    }

  } else {
    if (column != 0) {
      DBG_PRINT_MSG(
          "Error: %s",
          "LinearLayout contains only 1 column, and the 2nd parameter will be ignored");
    }

    if (InsertSubView(row, widget)) {
      Adjust();
      RequestRedraw();
      return widget;
    }

  }

  return 0;
}

void AdaptiveLayout::Adjust ()
{
  int x = margin().left();
  int y = margin().bottom();
  int width = size().width() - margin().hsum();
  int height = size().height() - margin().vsum();

  AdaptiveAdjustment adjustment(this, orientation_, alignment_, space_);
  adjustment.Adjust(x, y, width, height);
}

Size AdaptiveLayout::GetPreferredSize () const
{
  if (subview_count() == 0) {
    return Size(200, 200);
  }

  int w = 0;
  int h = 0;

  Size tmp;

  if (orientation_ == Horizontal) {
    w = -space_;
    for (AbstractView* p = first(); p; p = next(p)) {
      tmp = p->GetPreferredSize();

      w += (tmp.width() + space_);
      h = std::max(h, tmp.height());
    }

  } else {
    h = -space_;
    for (AbstractView* p = first(); p; p = next(p)) {
      tmp = p->GetPreferredSize();

      w = std::max(w, tmp.width());
      h += (tmp.height() + space_);
    }
  }

  w += pixel_size(margin().hsum());
  h += pixel_size(margin().vsum());

  return Size(w, h);
}

bool AdaptiveLayout::IsExpandX () const
{
  bool expand = false;

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandX()) {
      expand = true;
      break;
    }
  }

  return expand;
}

bool AdaptiveLayout::IsExpandY () const
{
  bool expand = false;

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandY()) {
      expand = true;
      break;
    }
  }

  return expand;
}

bool AdaptiveLayout::SizeUpdateTest (const AbstractView* source,
                                     const AbstractView* target,
                                     int width,
                                     int height) const
{
  return true;
}

bool AdaptiveLayout::PositionUpdateTest (const AbstractView* source,
                                         const AbstractView* target,
                                         int x,
                                         int y) const
{
  return true;
}

void AdaptiveLayout::PerformMarginUpdate (const Margin& margin)
{
  set_margin(margin);

  if (subview_count()) {
    Adjust();
    RequestRedraw();
  }
}

void AdaptiveLayout::PerformSizeUpdate (const AbstractView* source,
                                        const AbstractView* target,
                                        int width,
                                        int height)
{
  if (target == this) {
    set_size(width, height);
    Adjust();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  } else if (source->super() == this) {
    // a sub view resized
    Adjust();
  }
}

}
