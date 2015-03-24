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

#include <gui/stack-layout.hpp>

namespace BlendInt {

  StackLayout::StackLayout ()
  : AbstractLayout(), active_widget_(0)
  {
  }

  StackLayout::~StackLayout ()
  {
  }

  bool StackLayout::AddWidget (AbstractWidget* widget)
  {
    if (PushBackSubView(widget)) {
      int w = size().width() - margin().hsum();
      int h = size().height() - margin().vsum();

      ResizeSubView(widget, w, h);
      MoveSubViewTo(widget, margin().left(), margin().bottom());

      if (subview_count() == 1) {
        active_widget_ = widget;
      }

      return true;
    }

    return false;
  }

  bool StackLayout::InsertWidget (int index, AbstractWidget* widget)
  {
    if (InsertSubView(index, widget)) {
      int w = size().width() - margin().hsum();
      int h = size().height() - margin().vsum();

      ResizeSubView(widget, w, h);
      MoveSubViewTo(widget, margin().left(), margin().bottom());

      RequestRedraw();
      return true;
    }

    return false;
  }

  bool StackLayout::InsertWidget (int row, int column, AbstractWidget* widget)
  {
    if (PushFrontSubView(widget)) {
      int w = size().width() - margin().hsum();
      int h = size().height() - margin().vsum();

      ResizeSubView(widget, w, h);
      MoveSubViewTo(widget, margin().left(), margin().bottom());

      if (subview_count() == 1) {
        active_widget_ = widget;
      }

      RequestRedraw();
      return true;
    }

    return false;
  }

  void StackLayout::Adjust () const
  {

  }

  void StackLayout::Remove (AbstractWidget* widget)
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

  int StackLayout::GetIndex () const
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

  void StackLayout::SetIndex (int index)
  {
    int count = subview_count();

    if (index > (count - 1)) return;

    if (count) {

      AbstractView* widget = GetSubViewAt(index);
      if (active_widget_ == widget) {
        return;
      }

      active_widget_ = dynamic_cast<AbstractWidget*>(widget);
    }
  }

  bool StackLayout::IsExpandX () const
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

  bool StackLayout::IsExpandY () const
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

  Size StackLayout::GetPreferredSize () const
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

  void StackLayout::PerformMarginUpdate (const Margin& request)
  {
    int w = size().width() - request.hsum();
    int h = size().height() - request.vsum();

    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, w, h);
    }
  }

  void StackLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      int w = request.size()->width() - margin().hsum();
      int h = request.size()->height() - margin().vsum();

      set_size(*request.size());

      for (AbstractView* p = first(); p; p = next(p)) {
        ResizeSubView(p, w, h);
      }

    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  AbstractView* StackLayout::GetFirstSubView () const
  {
    return active_widget_;
  }

  AbstractView* StackLayout::GetLastSubView () const
  {
    return active_widget_;
  }

  AbstractView* StackLayout::GetNextSubView (const AbstractView* view) const
  {
    return 0;
  }

  AbstractView* StackLayout::GetPreviousSubView (const AbstractView* view) const
  {
    return 0;
  }

  int StackLayout::GetSubViewCount () const
  {
    return active_widget_ ? 1 : 0;
  }

  bool StackLayout::IsSubViewActive (const AbstractView* subview) const
  {
    DBG_ASSERT(subview && subview->super() == this);
    return subview == active_widget_ ? true : false;
  }

}
