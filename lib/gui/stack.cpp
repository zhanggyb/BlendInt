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

#include <blendint/gui/stack.hpp>

namespace BlendInt {

Stack::Stack ()
    : AbstractWidget(), active_widget_(0)
{
  set_size(400, 300);
}

Stack::~Stack ()
{
}

void Stack::AddWidget (AbstractWidget* widget)
{
  if (PushBackSubView(widget)) {

    ResizeSubView(widget, size());
    MoveSubViewTo(widget, 0, 0);

    if (subview_count() == 1) {
      active_widget_ = widget;
    }

    RequestRedraw();
  }
}

void Stack::InsertWidget (int index, AbstractWidget* widget)
{
  if (InsertSubView(index, widget)) {

    ResizeSubView(widget, size());
    MoveSubViewTo(widget, 0, 0);

    RequestRedraw();
  }
}

void Stack::Remove (AbstractWidget* widget)
{
  if (RemoveSubView(widget)) {

    if (active_widget_ == widget) {

      if (subview_count() == 0) {
        active_widget_ = 0;
      } else {
        active_widget_ = dynamic_cast<AbstractWidget*>(first());
      }

    }

    RequestRedraw();
  }
}

int Stack::GetIndex () const
{
  int index = 0;

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p == active_widget_) {
      break;
    }

    index++;
  }

  if (index >= subview_count()) index = -1;

  return index;
}

void Stack::SetIndex (int index)
{
  int count = subview_count();

  if ((index < 0) || (index > (count - 1))) return;

  if (count) {

    AbstractView* widget = GetSubViewAt(index);
    if (active_widget_ == widget) {
      return;
    }

    //active_widget_->SetVisible(false);
    active_widget_ = dynamic_cast<AbstractWidget*>(widget);
    //active_widget_->SetVisible(true);
  }
}

bool Stack::IsExpandX () const
{
  bool ret = false;

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandX()) {
      ret = true;
      break;
    }
  }

  return ret;
}

bool Stack::IsExpandY () const
{
  bool ret = false;

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandY()) {
      ret = true;
      break;
    }
  }

  return ret;
}

Size Stack::GetPreferredSize () const
{
  if (subview_count()) {

    int w = 0;
    int h = 0;

    Size tmp;
    for (AbstractView* p = first(); p; p = next(p)) {
      tmp = p->GetPreferredSize();
      w = std::max(w, tmp.width());
      h = std::max(h, tmp.height());
    }

    return Size(w, h);

  } else {
    return Size(400, 300);
  }
}

AbstractView* Stack::GetFirstSubView () const
{
  return active_widget_;
}

AbstractView* Stack::GetLastSubView () const
{
  return active_widget_;
}

AbstractView* Stack::GetNextSubView (const AbstractView* view) const
{
  return 0;
}

AbstractView* Stack::GetPreviousSubView (const AbstractView* view) const
{
  return 0;
}

int Stack::GetSubViewCount () const
{
  return active_widget_ ? 1 : 0;
}

bool Stack::IsSubViewActive (const AbstractView* subview) const
{
  DBG_ASSERT(subview && subview->super() == this);
  return subview == active_widget_ ? true : false;
}

void Stack::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if (target == this) {
    set_size(width, height);
    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, size());
    }
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

Response Stack::Draw (AbstractWindow* context)
{
  return subview_count() ? Ignore : Finish;
}

}
