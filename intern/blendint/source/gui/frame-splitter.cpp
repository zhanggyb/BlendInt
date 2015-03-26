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

#include <core/types.hpp>

#include <gui/frame-splitter.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  FrameSplitterHandle::FrameSplitterHandle (Orientation orientation)
  : AbstractFrame(FrameRegular),
    orientation_(orientation),
    prev_size_(0),
    next_size_(0),
    nearby_pos_(0),
    hover_(false),
    pressed_(false)
  {
    if (orientation == Horizontal) {
      set_size(200, 1);
    } else {
      set_size(1, 200);
    }
  }

  FrameSplitterHandle::~FrameSplitterHandle ()
  {
  }

  bool FrameSplitterHandle::Contain (const Point& point) const
  {
    if (orientation_ == Horizontal) {

      if (point.x() < position().x() || point.y() < (position().y() - 2)
          || point.x() > static_cast<int>(position().x() + size().width())
          || point.y()
              > static_cast<int>(position().y() + size().height() + 2)) {
        return false;
      }

    } else {

      if (point.x() < (position().x() - 2) || point.y() < position().y()
          || point.x() > static_cast<int>(position().x() + size().width() + 2)
          || point.y() > static_cast<int>(position().y() + size().height())) {
        return false;
      }

    }

    return true;
  }

  bool FrameSplitterHandle::IsExpandX () const
  {
    if (orientation_ == Horizontal) {
      return true;
    } else {
      return false;
    }
  }

  bool FrameSplitterHandle::IsExpandY () const
  {
    if (orientation_ == Vertical) {
      return true;
    } else {
      return false;
    }
  }

  Size FrameSplitterHandle::GetPreferredSize () const
  {
    if (orientation_ == Horizontal) {
      return Size(200, 1);
    } else {
      return Size(1, 200);
    }
  }

  bool FrameSplitterHandle::PreDraw (AbstractWindow* context)
  {
    return false;
  }

  Response FrameSplitterHandle::Draw (AbstractWindow* context)
  {
    return Finish;
  }

  void FrameSplitterHandle::PostDraw (AbstractWindow* context)
  {
  }

  void FrameSplitterHandle::PerformFocusOn (AbstractWindow* context)
  {
  }

  void FrameSplitterHandle::PerformFocusOff (AbstractWindow* context)
  {
  }

  Response FrameSplitterHandle::PerformKeyPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response FrameSplitterHandle::PerformContextMenuPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response FrameSplitterHandle::PerformContextMenuRelease (AbstractWindow* context)
  {
    return Ignore;
  }

  Response FrameSplitterHandle::PerformMousePress (AbstractWindow* context)
  {
    last_ = position();
    cursor_ = context->GetGlobalCursorPosition();

    if (orientation_ == Horizontal) {
      prev_size_ = previous(this)->size().height();
      next_size_ = next(this)->size().height();
      nearby_pos_ = previous(this)->position().y();
    } else {
      prev_size_ = previous(this)->size().width();
      next_size_ = next(this)->size().width();
      nearby_pos_ = next(this)->position().x();
    }

    pressed_ = true;

    return Finish;
  }

  Response FrameSplitterHandle::PerformMouseRelease (AbstractWindow* context)
  {
    if (!hover_) {
      context->PopCursor();
    }

    pressed_ = false;
    return Finish;
  }

  void FrameSplitterHandle::PerformHoverIn (AbstractWindow* context)
  {
    hover_ = true;

    context->PushCursor();
    context->SetCursor(orientation_ == Horizontal ? SplitVCursor : SplitHCursor);
  }

  void FrameSplitterHandle::PerformHoverOut (AbstractWindow* context)
  {
    hover_ = false;
    if (!pressed_) context->PopCursor();
  }

  Response FrameSplitterHandle::PerformMouseHover (AbstractWindow* context)
  {
    if (Contain(context->GetGlobalCursorPosition())) {
      return Finish;
    } else {
      return Ignore;
    }
  }

  Response FrameSplitterHandle::PerformMouseMove (AbstractWindow* context)
  {
    if (pressed_) {

      FrameSplitter* splitter = dynamic_cast<FrameSplitter*>(super());
      DBG_ASSERT(splitter);

      if (orientation_ == Horizontal) {

        int offset = context->GetGlobalCursorPosition().y() - cursor_.y();
        int oy1 = prev_size_ - offset;
        int oy2 = next_size_ + offset;

        if ((oy1 <= 0) || (oy2 <= 0)) {
          return Finish;
        }

        splitter->MoveSubViewTo(this, last_.x(), last_.y() + offset);

        splitter->ResizeSubView(previous(this),
                                previous(this)->size().width(), oy1);
        splitter->MoveSubViewTo(previous(this),
                                previous(this)->position().x(),
                                nearby_pos_ + offset);
        splitter->ResizeSubView(next(this), next(this)->size().width(), oy2);

      } else {

        int offset = context->GetGlobalCursorPosition().x() - cursor_.x();
        int oy1 = prev_size_ + offset;
        int oy2 = next_size_ - offset;

        if ((oy1 <= 0) || (oy2 <= 0)) {
          return Finish;
        }

        splitter->MoveSubViewTo(this, last_.x() + offset, last_.y());

        splitter->ResizeSubView(previous(this), oy1,
                                previous(this)->size().height());
        splitter->ResizeSubView(next(this), oy2, next(this)->size().height());
        splitter->MoveSubViewTo(next(this), nearby_pos_ + offset,
                                next(this)->position().y());

      }

      RequestRedraw();
      return Finish;
    }
    return Finish;
  }

  // --------------------------------

  FrameSplitter::FrameSplitter (Orientation orientation)
  : AbstractFrame(FrameRegular),
    orientation_(orientation),
    hover_frame_(0),
    focused_frame_(0),
    focused_(false),
    pressed_(false),
    hover_(false),
    cursor_position_(0)
  {
    set_size(500, 500);
  }

  FrameSplitter::~FrameSplitter ()
  {

  }

  bool FrameSplitter::AddFrame (AbstractFrame* frame, SizePolicy policy)
  {
    if ((frame == 0) || (frame->super() == this)) return false;

    if (subview_count() == 0) {
      PushBackSubView(frame);
      AlignSubFrames(orientation_, size());
    } else {

      if (orientation_ == Horizontal) {
        AddColumn(frame, policy);
      } else {
        AddRow(frame, policy);
      }

    }

    RequestRedraw();

    return true;
  }

  bool FrameSplitter::InsertFrame (int index,
                                   AbstractFrame* frame,
                                   SizePolicy policy)
  {
    if ((frame == 0) || (frame->super() == this)) return false;

    if (subview_count() == 0) {
      PushBackSubView(frame);
      AlignSubFrames(orientation_, size());
    } else {

      if (orientation_ == Horizontal) {
        InsertColumn(index, frame, policy);
      } else {
        InsertRow(index, frame, policy);
      }

    }

    RequestRedraw();

    return true;
  }

  int FrameSplitter::GetFrameIndex (AbstractFrame* frame) const
  {
    if (frame->super() != this) return -1;

    int index = 0;
    for (AbstractView* p = first(); p; p = next(p)) {
      if (p == frame) break;

      index++;
      p = next(next(p));
    }

    return index;
  }

  int FrameSplitter::GetHandleIndex (FrameSplitterHandle* handle) const
  {
    if (handle->super() != this) return -1;

    int index = 0;
    for (AbstractView* p = next(first()); p; p = next(p)) {
      if (p == handle) break;

      index++;
      p = next(next(p));
    }

    return index;
  }

  AbstractFrame* FrameSplitter::GetFrame (int index) const
  {
    if (subview_count() == 0) return 0;

    int max = (subview_count() + 1) / 2;
    if (index > max) return 0;

    index = index * 2;

    return dynamic_cast<AbstractFrame*>(GetSubViewAt(index));
  }

  FrameSplitterHandle* FrameSplitter::GetHandle (int index) const
  {
    if (subview_count() <= 1) return 0;

    int max = (subview_count() - 1) / 2;
    if (index > max) return 0;

    index = index * 2 + 1;

    return dynamic_cast<FrameSplitterHandle*>(GetSubViewAt(index));
  }

  int FrameSplitter::GetFramesCount () const
  {
    int sum = subview_count();
    return (sum / 2 + 1);
  }

  bool FrameSplitter::IsExpandX () const
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

  bool FrameSplitter::IsExpandY () const
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

  Size FrameSplitter::GetPreferredSize () const
  {
    Size preferred_size;

    if (first() == 0) {
      preferred_size.set_width(400);
      preferred_size.set_height(400);
    } else {
      Size tmp;

      if (orientation_ == Horizontal) {
        for (AbstractView* p = first(); p; p = next(p)) {
          tmp = p->GetPreferredSize();
          preferred_size.add_width(tmp.width());
          preferred_size.set_height(
              std::max(preferred_size.height(), tmp.height()));
        }
      } else {
        for (AbstractView* p = first(); p; p = next(p)) {
          tmp = p->GetPreferredSize();
          preferred_size.add_height(tmp.height());
          preferred_size.set_width(
              std::max(preferred_size.width(), tmp.width()));
        }
      }

    }

    return preferred_size;
  }

  void FrameSplitter::PerformPositionUpdate (const AbstractView* source, const AbstractView* target, int x, int y)
  {
    if (target == this) {
      int ox = x - position().x();
      int oy = y - position().y();

      set_position(x, y);

      for (AbstractView* p = first(); p; p = next(p)) {
        MoveSubViewTo(p, p->position().x() + ox, p->position().y() + oy);
      }

    }

    if (source == this) {
      report_position_update(source, target, x, y);
    }
  }

  void FrameSplitter::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
  {
    if (target == this) {
      set_size(width, height);
      FillSubFrames();
    }

    if (source == this) {
      report_size_update(source, target, width, height);
    }
  }

  bool FrameSplitter::PreDraw (AbstractWindow* context)
  {
    return true;
  }

  Response FrameSplitter::Draw (AbstractWindow* context)
  {
    DrawSubViewsOnce(context);

    return Finish;
  }

  void FrameSplitter::PostDraw (AbstractWindow* context)
  {
  }

  void FrameSplitter::AlignSubFrames (Orientation orientation, const Size& size)
  {
    int room = GetAverageRoom(orientation, size);
    int x = position().x();

    if (orientation == Horizontal) {

      int y = position().y();
      int h = size.height();

      int i = 0;
      int handler_width = 0;
      for (AbstractView* p = first(); p; p = next(p)) {
        if (i % 2 == 0) {
          ResizeSubView(p, room, h);
          MoveSubViewTo(p, x, y);
          x = x + room;
        } else {
          handler_width = p->GetPreferredSize().width();
          ResizeSubView(p, handler_width, h);
          MoveSubViewTo(p, x, y);
          x = x + handler_width;
        }
        i++;
      }

    } else {

      int y = position().y() + this->size().height();
      int w = size.width();

      int i = 0;
      int handler_height = 0;
      for (AbstractView* p = first(); p; p = next(p)) {
        if (i % 2 == 0) {
          y = y - room;
          ResizeSubView(p, w, room);
          MoveSubViewTo(p, x, y);
        } else {
          handler_height = p->GetPreferredSize().height();
          y = y - handler_height;
          ResizeSubView(p, w, handler_height);
          MoveSubViewTo(p, x, y);
        }

        i++;
      }

    }
  }

  void FrameSplitter::PerformFocusOn (AbstractWindow* context)
  {
    focused_ = true;
  }

  void FrameSplitter::PerformFocusOff (AbstractWindow* context)
  {
    focused_ = false;

    if (focused_frame_) {
      dispatch_focus_off(focused_frame_, context);
      focused_frame_ = 0;
    }
  }

  void FrameSplitter::PerformHoverIn (AbstractWindow* context)
  {
    hover_ = true;
  }

  void FrameSplitter::PerformHoverOut (AbstractWindow* context)
  {
    hover_ = false;

    if (hover_frame_) {
      dispatch_mouse_hover_out(hover_frame_, context);
      hover_frame_ = nullptr;
    }
  }

  Response FrameSplitter::PerformKeyPress (AbstractWindow* context)
  {
    if (focused_frame_) {
      return dispatch_key_press(focused_frame_, context);
    }

    return Ignore;
  }

  Response FrameSplitter::PerformMousePress (AbstractWindow* context)
  {
    Response response = Ignore;

    if(cursor_position_ == InsideRectangle) {

      if (context == super()) context->SetFocusedFrame(this);

      pressed_ = true;

      if (hover_frame_ != nullptr) {
        response = dispatch_mouse_press(hover_frame_, context);

        if (response == Finish) {
          SetFocusedFrame(hover_frame_, context);
        }
      } else {
        SetFocusedFrame(0, context);
      }

      return Finish;

    } else {
      pressed_ = false;
      return Ignore;
    }
  }

  Response FrameSplitter::PerformMouseRelease (AbstractWindow* context)
  {
    Response response = Ignore;
    pressed_ = false;

    if (focused_frame_ != nullptr) {
      response = dispatch_mouse_release(focused_frame_, context);
    }

    return response;
  }

  Response FrameSplitter::PerformMouseMove (AbstractWindow* context)
  {
    Response response = Ignore;

    if (pressed_) {
      if (focused_frame_)
        response = dispatch_mouse_move(focused_frame_, context);
    } else {

      if (context->mouse_tracking() && focused_frame_) {
        response = dispatch_mouse_move(focused_frame_, context);
      }

    }

    return response;
  }

  Response FrameSplitter::PerformMouseHover (AbstractWindow* context)
  {
    if (Contain(context->GetGlobalCursorPosition())) {

      if(context->active_frame() && (context == super())) {
        // a frame is hovered above this cursor position

        if (hover_) {
          DeclareActiveFrame(context, this);
          PerformHoverOut(context);
          hover_ = false;
        }

        return Finish;
      }

      cursor_position_ = InsideRectangle;

      if (!hover_) PerformHoverIn(context);

      Response response = Finish;
      SetHoveredFrame(context);
      if (hover_frame_) {
        response = dispatch_mouse_hover(hover_frame_, context);
      }

      //return response;
      return Ignore;

    } else {

      cursor_position_ = OutsideRectangle;

      if (hover_) PerformHoverOut(context);

      if (pressed_) return Finish;
      else return Ignore;
    }
  }

  void FrameSplitter::FillSubFrames ()
  {
    if (orientation_ == Horizontal) {

      DistributeHorizontally();
      AlignHorizontally();

    } else {

      DistributeVertically();
      AlignVertically();

    }
  }

  int FrameSplitter::GetAverageRoom (Orientation orientation, const Size& size)
  {
    int retval = 0;

    if (orientation == Horizontal) {
      retval = size.width();
    } else {
      retval = size.height();
    }

    if (first() == 0) {
      return retval;
    }

    Size prefer;
    int space = 0;
    int sum = 1;

    // get all the total width/height of splitter handlers
    AbstractView* p = next(first());
    while (p) {
      prefer = p->GetPreferredSize();
      if (orientation == Horizontal) {
        space = prefer.width();
      } else {
        space = prefer.height();
      }

      p = next(next(p));
      sum += 2;
    }

    retval = retval - space;
    retval = retval / ((sum + 1) / 2);

    return retval;
  }

  void FrameSplitter::AddColumn (AbstractFrame* frame, SizePolicy policy)
  {
#ifdef DEBUG
    DBG_ASSERT(orientation_ == Horizontal);
#endif

    Size squeezed_size = size();
    Size frame_size = frame->size();
    FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Vertical));

    switch (policy) {
      case PreferredWidth: {
        frame_size.set_width(frame->GetPreferredSize().width());
        break;
      }
      case CurrentWidth: {
        break;
      }
      case ExpandX: {
        int w = 0;
        for (AbstractView* p = first(); p; p = next(p)) {
          w = p->GetPreferredSize().width();
        }
        frame_size.set_height(size().width() - handle->size().width() - w);
        break;
      }
      default: {
        break;
      }
    }
    squeezed_size.set_width(
        squeezed_size.width() - handle->size().width() - frame_size.width());

    if ((squeezed_size.width() < 0) || (policy == DefaultSizePolicy)) {
      PushBackSubView(handle);
      PushBackSubView(frame);
      AlignSubFrames(orientation_, size());
    } else {
      AlignSubFrames(orientation_, squeezed_size);

      Point pos = last()->position();
      pos.set_x(pos.x() + last()->size().width());

      PushBackSubView(handle);
      PushBackSubView(frame);

      ResizeSubView(handle, handle->size().width(), size().height());
      frame_size.set_height(size().height());
      ResizeSubView(frame, frame_size);

      MoveSubViewTo(handle, pos);
      pos.set_x(pos.x() + handle->size().width());
      MoveSubViewTo(frame, pos);

    }

  }

  void FrameSplitter::AddRow (AbstractFrame* frame, SizePolicy policy)
  {
#ifdef DEBUG
    DBG_ASSERT(orientation_ == Vertical);
#endif

    Size squeezed_size = size();
    Size frame_size = frame->size();
    FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Horizontal));

    switch (policy) {
      case PreferredHeight: {
        frame_size.set_height(frame->GetPreferredSize().height());
        break;
      }
      case CurrentHeight: {
        break;
      }
      case ExpandY: {
        int h = 0;
        for (AbstractView* p = first(); p; p = next(p)) {
          h = p->GetPreferredSize().height();
        }
        frame_size.set_height(size().height() - handle->size().height() - h);
        break;
      }

      default: {
        break;
      }
    }
    squeezed_size.set_height(
        size().height() - handle->size().height() - frame_size.height());

    if ((squeezed_size.height() < 0) || (policy == DefaultSizePolicy)) {
      PushBackSubView(handle);
      PushBackSubView(frame);
      AlignSubFrames(orientation_, size());
    } else {
      AlignSubFrames(orientation_, squeezed_size);

      Point pos = last()->position();

      PushBackSubView(handle);
      PushBackSubView(frame);

      ResizeSubView(handle, size().width(), handle->size().height());
      frame_size.set_width(size().width());
      ResizeSubView(frame, frame_size);

      pos.set_y(pos.y() - handle->size().height());
      MoveSubViewTo(handle, pos);
      pos.set_y(pos.y() - frame->size().height());
      MoveSubViewTo(frame, pos);

    }

  }

  void FrameSplitter::InsertColumn (int index,
                                    AbstractFrame* frame,
                                    SizePolicy policy)
  {
#ifdef DEBUG
    DBG_ASSERT(orientation_ == Horizontal);
#endif

    Size squeezed_size = size();
    Size frame_size = frame->size();
    FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Vertical));

    switch (policy) {
      case PreferredWidth: {
        frame_size.set_width(frame->GetPreferredSize().width());
        break;
      }
      case CurrentWidth: {
        break;
      }
      case ExpandX: {
        int w = 0;
        for (AbstractView* p = first(); p; p = next(p)) {
          w = p->GetPreferredSize().width();
        }
        frame_size.set_height(size().width() - handle->size().width() - w);
        break;
      }
      default: {
        break;
      }
    }
    squeezed_size.set_width(
        squeezed_size.width() - handle->size().width() - frame_size.width());

    if ((squeezed_size.width() < 0) || (policy == DefaultSizePolicy)) {

      index = index * 2;

      if (index > (subview_count() - 1)) {
        // append
        InsertSubView(index, handle);
        InsertSubView(index + 1, frame);
      } else {
        InsertSubView(index, frame);
        InsertSubView(index + 1, handle);
      }

      AlignSubFrames(orientation_, size());

    } else {

      index = index * 2;

      if (index > (subview_count() - 1)) {
        // append
        InsertSubView(index, handle);
        InsertSubView(index + 1, frame);
      } else {
        InsertSubView(index, frame);
        InsertSubView(index + 1, handle);
      }

      ResizeSubView(handle, handle->size().width(), size().height());
      frame_size.set_height(size().height());
      ResizeSubView(frame, frame_size);

      //AlignSubFrames(orientation_, squeezed_size);
      AlignSubFrames(orientation_, size());
    }
  }

  void FrameSplitter::InsertRow (int index,
                                 AbstractFrame* frame,
                                 SizePolicy policy)
  {
#ifdef DEBUG
    DBG_ASSERT(orientation_ == Vertical);
#endif

    Size squeezed_size = size();
    Size frame_size = frame->size();
    FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Horizontal));

    switch (policy) {
      case PreferredHeight: {
        frame_size.set_height(frame->GetPreferredSize().height());
        break;
      }
      case CurrentHeight: {
        break;
      }
      case ExpandY: {
        int h = 0;
        for (AbstractView* p = first(); p; p = next(p)) {
          h = p->GetPreferredSize().height();
        }
        frame_size.set_height(size().height() - handle->size().height() - h);
        break;
      }

      default: {
        break;
      }
    }
    squeezed_size.set_height(
        size().height() - handle->size().height() - frame_size.height());

    if ((squeezed_size.height() < 0) || (policy == DefaultSizePolicy)) {

      index = index * 2;

      if (index > (subview_count() - 1)) {
        // append
        InsertSubView(index, handle);
        InsertSubView(index + 1, frame);
      } else {
        InsertSubView(index, frame);
        InsertSubView(index + 1, handle);
      }

      AlignSubFrames(orientation_, size());

    } else {

      index = index * 2;

      if (index > (subview_count() - 1)) {
        // append
        InsertSubView(index, handle);
        InsertSubView(index + 1, frame);
      } else {
        InsertSubView(index, frame);
        InsertSubView(index + 1, handle);
      }

      ResizeSubView(handle, size().width(), handle->size().height());
      frame_size.set_width(size().width());
      ResizeSubView(frame, frame_size);

      //AlignSubFrames(orientation_, squeezed_size);
      AlignSubFrames(orientation_, size());

    }
  }

  void FrameSplitter::DistributeHorizontally ()
  {
    boost::scoped_ptr<std::deque<int> > expandable_widths(new std::deque<int>);
    boost::scoped_ptr<std::deque<int> > unexpandable_widths(
        new std::deque<int>);
    boost::scoped_ptr<std::deque<int> > handler_prefer_widths(
        new std::deque<int>);

    int expandable_width_sum = 0;	// the width sum of the expandable widgets' size
    int unexpandable_width_sum = 0;	// the width sum of the unexpandable widgets' size
    int handlers_width_sum = 0;

    int prefer_width;
    int i = 0;
    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {	// widgets

        if (p->IsExpandX()) {
          expandable_width_sum += p->size().width();
          expandable_widths->push_back(p->size().width());
        } else {
          unexpandable_width_sum += p->size().width();
          unexpandable_widths->push_back(p->size().width());
        }

      } else {	// handlers

        prefer_width = p->GetPreferredSize().width();
        handler_prefer_widths->push_back(prefer_width);
        handlers_width_sum += prefer_width;

      }

      i++;
    }

    if ((expandable_widths->size() + unexpandable_widths->size()) == 0) return;	// do nothing if all sub widgets are invisible

    if (expandable_widths->size() == 0) {

      DistributeHorizontallyInProportion(unexpandable_widths.get(),
                                         unexpandable_width_sum,
                                         handler_prefer_widths.get(),
                                         handlers_width_sum);

    } else if (unexpandable_widths->size() == 0) {

      DistributeHorizontallyInProportion(expandable_widths.get(),
                                         expandable_width_sum,
                                         handler_prefer_widths.get(),
                                         handlers_width_sum);

    } else {

      int exp_width = size().width() - handlers_width_sum
          - unexpandable_width_sum;

      if (exp_width <= 0) {

        DistributeUnexpandableFramesHorizontally(unexpandable_widths.get(),
                                                 unexpandable_width_sum,
                                                 handler_prefer_widths.get(),
                                                 handlers_width_sum);

      } else {

        DistributeExpandableFramesHorizontally(unexpandable_width_sum,
                                               expandable_widths.get(),
                                               expandable_width_sum,
                                               handler_prefer_widths.get(),
                                               handlers_width_sum);

      }

    }

  }

  void FrameSplitter::DistributeHorizontallyInProportion (std::deque<int>* widget_deque,
                                                          int widget_width_sum,
                                                          std::deque<int>* prefer_deque,
                                                          int prefer_width_sum)
  {
    int x = position().x();
    int i = 0;
    std::deque<int>::iterator width_it = widget_deque->begin();
    std::deque<int>::iterator handler_width_it = prefer_deque->begin();
    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        ResizeSubView(
            p,
            (size().width() - prefer_width_sum) * (*width_it)
                / widget_width_sum,
            p->size().height());
        width_it++;

      } else {

        ResizeSubView(p, *handler_width_it, p->size().height());
        handler_width_it++;
      }

      MoveSubViewTo(p, x, p->position().y());
      x += p->size().width();

      i++;
    }
  }

  void FrameSplitter::DistributeExpandableFramesHorizontally (int unexpandable_width_sum,
                                                              std::deque<int>* widget_deque,
                                                              int widget_width_sum,
                                                              std::deque<int>* prefer_deque,
                                                              int prefer_width_sum)
  {
    int x = position().x();
    int i = 0;
    std::deque<int>::iterator exp_width_it = widget_deque->begin();
    std::deque<int>::iterator handler_width_it = prefer_deque->begin();
    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        if (p->IsExpandX()) {
          ResizeSubView(
              p,
              (size().width() - prefer_width_sum - unexpandable_width_sum)
                  * (*exp_width_it) / widget_width_sum,
              p->size().height());
          exp_width_it++;
        }

      } else {
        ResizeSubView(p, *handler_width_it, p->size().height());
        handler_width_it++;
      }

      MoveSubViewTo(p, x, p->position().y());
      x += p->size().width();

      i++;
    }
  }

  void FrameSplitter::DistributeUnexpandableFramesHorizontally (std::deque<int>* widget_deque,
                                                                int widget_width_sum,
                                                                std::deque<int>* prefer_deque,
                                                                int prefer_width_sum)
  {
    int x = position().x();
    int i = 0;
    std::deque<int>::iterator unexp_width_it = widget_deque->begin();
    std::deque<int>::iterator handler_width_it = prefer_deque->begin();
    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        if (!p->IsExpandX()) {

          ResizeSubView(
              p,
              (size().width() - prefer_width_sum) * (*unexp_width_it)
                  / widget_width_sum,
              p->size().height());
          unexp_width_it++;

        }

      } else {
        ResizeSubView(p, *handler_width_it, p->size().height());
        handler_width_it++;
      }

      MoveSubViewTo(p, x, p->position().y());
      x += p->size().width();

      i++;
    }
  }

  void FrameSplitter::DistributeVertically ()
  {
    boost::scoped_ptr<std::deque<int> > expandable_heights(new std::deque<int>);
    boost::scoped_ptr<std::deque<int> > unexpandable_heights(
        new std::deque<int>);
    boost::scoped_ptr<std::deque<int> > handler_prefer_heights(
        new std::deque<int>);

    int expandable_height_sum = 0;// the width sum of the expandable widgets' size
    int unexpandable_height_sum = 0;// the width sum of the unexpandable widgets' size
    int handlers_height_sum = 0;

    int prefer_height;
    int i = 0;
    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {	// widgets

        if (p->IsExpandY()) {
          expandable_height_sum += p->size().height();
          expandable_heights->push_back(p->size().height());
        } else {
          unexpandable_height_sum += p->size().height();
          unexpandable_heights->push_back(p->size().height());
        }

      } else {	// handlers

        prefer_height = p->GetPreferredSize().height();
        handler_prefer_heights->push_back(prefer_height);
        handlers_height_sum += prefer_height;

      }

      i++;
    }

    if ((expandable_heights->size() + unexpandable_heights->size()) == 0)
      return;	// do nothing if all sub widgets are invisible

    if (expandable_heights->size() == 0) {

      DistributeVerticallyInProportion(unexpandable_heights.get(),
                                       unexpandable_height_sum,
                                       handler_prefer_heights.get(),
                                       handlers_height_sum);

    } else if (unexpandable_heights->size() == 0) {

      DistributeVerticallyInProportion(expandable_heights.get(),
                                       expandable_height_sum,
                                       handler_prefer_heights.get(),
                                       handlers_height_sum);

    } else {

      int exp_height = size().height() - handlers_height_sum
          - unexpandable_height_sum;

      if (exp_height <= 0) {

        DistributeUnexpandableFramesVertically(unexpandable_heights.get(),
                                               unexpandable_height_sum,
                                               handler_prefer_heights.get(),
                                               handlers_height_sum);

      } else {

        DistributeExpandableFramesVertically(unexpandable_height_sum,
                                             expandable_heights.get(),
                                             expandable_height_sum,
                                             handler_prefer_heights.get(),
                                             handlers_height_sum);

      }

    }
  }

  void FrameSplitter::DistributeVerticallyInProportion (std::deque<int>* widget_deque,
                                                        int widget_height_sum,
                                                        std::deque<int>* prefer_deque,
                                                        int prefer_height_sum)
  {
    int y = position().y();
    int i = 0;
    std::deque<int>::iterator height_it = widget_deque->begin();
    std::deque<int>::iterator handler_height_it = prefer_deque->begin();

    y = y + size().height();

    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        ResizeSubView(
            p,
            p->size().width(),
            (size().height() - prefer_height_sum) * (*height_it)
                / widget_height_sum);
        height_it++;

      } else {

        ResizeSubView(p, p->size().width(), *handler_height_it);
        handler_height_it++;
      }

      y = y - p->size().height();
      MoveSubViewTo(p, p->position().x(), y);

      i++;
    }
  }

  void FrameSplitter::DistributeExpandableFramesVertically (int unexpandable_height_sum,
                                                            std::deque<int>* widget_deque,
                                                            int widget_height_sum,
                                                            std::deque<int>* prefer_deque,
                                                            int prefer_height_sum)
  {
    int y = position().y();
    int i = 0;
    std::deque<int>::iterator exp_height_it = widget_deque->begin();
    std::deque<int>::iterator handler_height_it = prefer_deque->begin();
    y = y + size().height();

    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        if (p->IsExpandY()) {
          ResizeSubView(
              p,
              p->size().width(),
              (size().height() - prefer_height_sum - unexpandable_height_sum)
                  * (*exp_height_it) / widget_height_sum);
          exp_height_it++;
        }

      } else {
        ResizeSubView(p, p->size().width(), *handler_height_it);
        handler_height_it++;
      }

      y -= p->size().height();
      MoveSubViewTo(p, p->position().x(), y);

      i++;
    }
  }

  void FrameSplitter::DistributeUnexpandableFramesVertically (std::deque<int>* widget_deque,
                                                              int widget_height_sum,
                                                              std::deque<int>* prefer_deque,
                                                              int prefer_height_sum)
  {
    int y = position().y();
    int i = 0;
    std::deque<int>::iterator unexp_height_it = widget_deque->begin();
    std::deque<int>::iterator handler_height_it = prefer_deque->begin();
    y = y + size().height();

    for (AbstractView* p = first(); p; p = next(p)) {
      if (i % 2 == 0) {

        if (!p->IsExpandY()) {

          ResizeSubView(
              p,
              p->size().width(),
              (size().height() - prefer_height_sum) * (*unexp_height_it)
                  / widget_height_sum);
          unexp_height_it++;

        }

      } else {
        ResizeSubView(p, p->size().width(), *handler_height_it);
        handler_height_it++;
      }

      y -= p->size().width();
      MoveSubViewTo(p, p->position().x(), y);

      i++;
    }
  }

  void FrameSplitter::AlignHorizontally ()
  {
    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, p->size().width(), size().height());
      MoveSubViewTo(p, p->position().x(), position().y());
    }
  }

  void FrameSplitter::AlignHorizontallyExt ()
  {
    int x = position().x();
    int y = position().y();

    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, p->size().width(), size().height());
      MoveSubViewTo(p, x, y);
      x += p->size().width();
    }
  }

  void FrameSplitter::AlignVertically ()
  {
    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, size().width(), p->size().height());
      MoveSubViewTo(p, position().x(), p->position().y());
    }
  }

  void FrameSplitter::AlignVerticallyExt ()
  {
    int x = position().x();
    int y = position().y() + size().height();

    for (AbstractView* p = first(); p; p = next(p)) {
      ResizeSubView(p, size().width(), p->size().height());

      y = y - p->size().height();
      MoveSubViewTo(p, x, y);
    }
  }

  void FrameSplitter::SetFocusedFrame (AbstractFrame* frame,
                                       AbstractWindow* context)
  {
    if (focused_frame_ == frame) return;

    if (focused_frame_ != nullptr) {
      dispatch_focus_off(focused_frame_, context);
    }

    focused_frame_ = frame;
    if (focused_frame_ != nullptr) {
      dispatch_focus_on(focused_frame_, context);
    }
  }

  AbstractView* FrameSplitter::RemoveSubView (AbstractView* view)
  {
    DBG_PRINT_MSG("%s", "Remove sub frame");

    if (view == focused_frame_) {
      focused_frame_ = nullptr;
    }

    if (view == hover_frame_) {
      hover_frame_ = nullptr;
    }

    // TODO: re-layout

    return AbstractFrame::RemoveSubView(view);
  }

  void FrameSplitter::SetHoveredFrame (AbstractWindow* context)
  {
    AbstractFrame* original = hover_frame_;

    if (hover_frame_) {

      if (!hover_frame_->Contain(context->GetGlobalCursorPosition())) {

        hover_frame_ = 0;

        // FrameSplitterHandle first:
        for (AbstractView* p = last(); p; p = previous(p)) {

          if (!previous(p)) break;
          p = previous(p);

          if (p->Contain(context->GetGlobalCursorPosition())) {
            hover_frame_ = dynamic_cast<AbstractFrame*>(p);
            break;
          }
        }

        // Then content frames:

        if (!hover_frame_) {
          for (AbstractView* p = last(); p;  p = previous(p)) {
            if (p->Contain(context->GetGlobalCursorPosition())) {
              hover_frame_ = dynamic_cast<AbstractFrame*>(p);
              break;
            }

            if (previous(p)) p = previous(p);
            else break;
          }
        }

      }

    } else {

      // FrameSplitterHandle first:
      for (AbstractView* p = last(); p; p = previous(p)) {

        if(!previous(p)) break;
        p = previous(p);

        if (p->Contain(context->GetGlobalCursorPosition())) {
          hover_frame_ = dynamic_cast<AbstractFrame*>(p);
          break;
        }
      }

      // Then content frames:

      if (!hover_frame_) {
        for (AbstractView* p = last(); p; p = previous(p)) {
          if (p->Contain(context->GetGlobalCursorPosition())) {
            hover_frame_ = dynamic_cast<AbstractFrame*>(p);
            break;
          }

          if (previous(p)) p = previous(p);
          else break;
        }
      }

    }

    if (original != hover_frame_) {

      if (original != nullptr) {
        dispatch_mouse_hover_out(original, context);
      }

      if (hover_frame_ != nullptr) {
        dispatch_mouse_hover_in(hover_frame_, context);
      }

    }
  }

}
