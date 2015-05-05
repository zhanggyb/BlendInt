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

#include <cassert>
#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/abstract-widget.hpp>
#include <blendint/gui/abstract-dialog.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

AbstractDialog::AbstractDialog (int dialog_flag)
: AbstractRoundFrame(FrameTopmost),
  focused_widget_(0),
  hovered_widget_(0),
  cursor_position_(InsideRectangle),
  dialog_flags_(dialog_flag & 0x0F),
  focused_(false),
  dragging_(false),
  pressed_(false)
{
}

AbstractDialog::AbstractDialog (int width, int height, int dialog_flag)
: AbstractRoundFrame(width, height, FrameTopmost),
  focused_widget_(0),
  hovered_widget_(0),
  cursor_position_(InsideRectangle),
  dialog_flags_(dialog_flag & 0x0F),
  focused_(false),
  dragging_(false),
  pressed_(false)
{
}

AbstractDialog::~AbstractDialog ()
{
  if (focused_widget_) {
    focused_widget_->destroyed().disconnect1(
        this, &AbstractDialog::OnFocusedWidgetDestroyed);
    focused_widget_ = 0;
  }

  if (hovered_widget_) {
    AbstractWindow* win = AbstractWindow::GetWindow(this);
    hovered_widget_->destroyed().disconnect1(
        this, &AbstractDialog::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, win);
  }
}

void AbstractDialog::PerformFocusOn (AbstractWindow* context)
{
  focused_ = true;
}

void AbstractDialog::PerformFocusOff (AbstractWindow* context)
{
  focused_ = false;

  if (focused_widget_) {
    focused_widget_->destroyed().disconnect1(
        this, &AbstractDialog::OnFocusedWidgetDestroyed);
    dispatch_focus_off(focused_widget_, context);
    focused_widget_ = 0;
  }
}

Response AbstractDialog::PerformKeyPress (AbstractWindow* context)
{
  Response response = Ignore;

  if ((context->GetKeyInput() == Key_Escape) && focused_) {

    if (context == super()) context->RequestRedraw();

    Destroy(this);

    return Finish;
  }

  if (focused_widget_) {
    DeclareActiveFrame(context, this);
    response = RecursiveDispatchKeyEvent(focused_widget_, context);
  }

  return response;
}

Response AbstractDialog::PerformMousePress (AbstractWindow* context)
{
  if (cursor_position_ == InsideRectangle) {

    context->SetFocusedFrame(this);

    pressed_ = true;
    last_position_ = position();
    cursor_point_ = context->GetGlobalCursorPosition();

    DeclareActiveFrame(context, this);
    if (hovered_widget_) {

      AbstractView* widget = RecursiveDispatchMousePress(hovered_widget_,
                                                         context);

      if (widget == 0) {
        dragging_ = true;
      } else {
        SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
      }

    } else {
      dragging_ = true;
    }

    return Finish;

  } else if (cursor_position_ != OutsideRectangle) {

    context->SetFocusedFrame(this);

    pressed_ = true;
    dragging_ = true;

    last_position_ = position();
    last_size_ = size();
    cursor_point_ = context->GetGlobalCursorPosition();

    return Finish;
  }

  pressed_ = false;
  if (modal()) {
    return Finish;
  }

  return Ignore;
}

Response AbstractDialog::PerformMouseRelease (AbstractWindow* context)
{
  pressed_ = false;

  Response result = Ignore;

  if (dragging_) {
    dragging_ = false;
    result = Finish;
  }

  cursor_position_ = InsideRectangle;

  if (focused_widget_) {
    DeclareActiveFrame(context, this);
    dispatch_mouse_release(focused_widget_, context);
  }

  return result;
}

Response AbstractDialog::PerformMouseMove (AbstractWindow* context)
{
  Response retval = Ignore;

  if (dragging_) {

    int ox = context->GetGlobalCursorPosition().x() - cursor_point_.x();
    int oy = context->GetGlobalCursorPosition().y() - cursor_point_.y();

    switch (cursor_position_) {

      case InsideRectangle: {
        set_position(last_position_.x() + ox, last_position_.y() + oy);
        break;
      }

      case OnLeftBorder: {
        set_position(last_position_.x() + ox, last_position_.y());
        Resize(last_size_.width() - ox, last_size_.height());
        break;
      }

      case OnRightBorder: {
        Resize(last_size_.width() + ox, last_size_.height());
        break;
      }

      case OnTopBorder: {
        Resize(last_size_.width(), last_size_.height() + oy);
        break;
      }

      case OnBottomBorder: {
        set_position(last_position_.x(), last_position_.y() + oy);
        Resize(last_size_.width(), last_size_.height() - oy);
        break;
      }

      case OnTopLeftCorner: {
        set_position(last_position_.x() + ox, last_position_.y());
        Resize(last_size_.width() - ox, last_size_.height() + oy);
        break;
      }

      case OnTopRightCorner: {
        Resize(last_size_.width() + ox, last_size_.height() + oy);
        break;
      }

      case OnBottomLeftCorner: {
        set_position(last_position_.x() + ox, last_position_.y() + oy);
        Resize(last_size_.width() - ox, last_size_.height() - oy);
        break;
      }

      case OnBottomRightCorner: {
        set_position(last_position_.x(), last_position_.y() + oy);
        Resize(last_size_.width() + ox, last_size_.height() - oy);
        break;
      }

      default: {
        return Finish;
        break;
      }

    }

    if (super()) {
      super()->RequestRedraw();
    }
    retval = Finish;

  } else {

    if (focused_widget_) {
      DeclareActiveFrame(context, this);
      retval = dispatch_mouse_move(focused_widget_, context);
    }

  }

  return retval;
}

Response AbstractDialog::PerformMouseHover (AbstractWindow* context)
{
  if (dragging_) return Finish;

  Response retval = Finish;
  int border = 4;

  Rect valid_rect(position().x() - border, position().y() - border,
                  size().width() + 2 * border, size().height() + 2 * border);

  if (valid_rect.contains(context->GetGlobalCursorPosition())) {

    if (Contain(context->GetGlobalCursorPosition())) {

      cursor_position_ = InsideRectangle;

      if (context->active_frame()) {

        DBG_ASSERT(context->active_frame() != this);

        // if there's frame above this position, dispatch hover out
        if (hovered_widget_) {
          DeclareActiveFrame(context, this);
          AbstractWindow* win = AbstractWindow::GetWindow(this);
          hovered_widget_->destroyed().disconnect1(
              this, &AbstractDialog::OnHoverWidgetDestroyed);
          ClearHoverWidgets(hovered_widget_, win);
          hovered_widget_ = 0;
        }

        retval = Finish;

      } else {

        AbstractWidget* new_hovered_widget = DispatchMouseHover(hovered_widget_,
                                                                context);

        if (new_hovered_widget != hovered_widget_) {

          if (hovered_widget_) {
            hovered_widget_->destroyed().disconnect1(
                this, &AbstractDialog::OnHoverWidgetDestroyed);
          }

          hovered_widget_ = new_hovered_widget;
          if (hovered_widget_) {
            hovered_widget_->destroyed().connect(
                this, &AbstractDialog::OnHoverWidgetDestroyed);
          }

        }

        // set cursor shape
        if (cursor_on_border()) {
          set_cursor_on_border(false);
          context->PopCursor();
        }

        DeclareActiveFrame(context, this);

        retval = Ignore;
      }

    } else {

      set_cursor_on_border(true);
      cursor_position_ = InsideRectangle;

      if (context->active_frame() == 0) {
        SetCursorShapeOnBorder(context);
        DeclareActiveFrame(context, this);
        retval = Ignore;
      } else {
        DBG_ASSERT(context->active_frame() != this);
        retval = Finish;
      }

    }

  } else {
    cursor_position_ = OutsideRectangle;

    // set cursor shape
    if (cursor_on_border()) {
      set_cursor_on_border(false);
      context->PopCursor();
    }

    if (pressed_) {
      retval = Finish;
    } else {
      retval = Ignore;
    }
  }

  // a modal dialog always return Finish
  if (modal()) {
    return Finish;
  } else {
    return retval;
  }
}

void AbstractDialog::SetFocusedWidget (AbstractWidget* widget,
                                       AbstractWindow* context)
{
  if (focused_widget_ == widget) return;

  if (focused_widget_) {
    dispatch_focus_off(focused_widget_, context);
    focused_widget_->destroyed().disconnect1(
        this, &AbstractDialog::OnFocusedWidgetDestroyed);
  }

  focused_widget_ = widget;
  if (focused_widget_) {
    dispatch_focus_on(focused_widget_, context);
    focused_widget_->destroyed().connect(
        this, &AbstractDialog::OnFocusedWidgetDestroyed);
  }
}

void AbstractDialog::OnFocusedWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(focused_widget_ == widget);

  //set_widget_focus_status(widget, false);
  DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
  widget->destroyed().disconnect1(this,
                                    &AbstractDialog::OnFocusedWidgetDestroyed);

  focused_widget_ = 0;
}

void AbstractDialog::OnHoverWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(hovered_widget_ == widget);

  DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
  widget->destroyed().disconnect1(this,
                                    &AbstractDialog::OnHoverWidgetDestroyed);

  hovered_widget_ = 0;
}

void AbstractDialog::SetCursorShapeOnBorder (AbstractWindow* context)
{
  if (context->GetGlobalCursorPosition().x() <= position().x()) {
    cursor_position_ |= OnLeftBorder;
  } else if (context->GetGlobalCursorPosition().x()
      >= (position().x() + size().width())) {
    cursor_position_ |= OnRightBorder;
  }

  if (context->GetGlobalCursorPosition().y()
      >= (position().y() + size().height())) {
    cursor_position_ |= OnTopBorder;
  } else if (context->GetGlobalCursorPosition().y() <= position().y()) {
    cursor_position_ |= OnBottomBorder;
  }

  // set cursor shape
  switch (cursor_position_) {

    case OnLeftBorder:
    case OnRightBorder: {
      context->PushCursor();
      context->SetCursor(SplitHCursor);

      break;
    }

    case OnTopBorder:
    case OnBottomBorder: {
      context->PushCursor();
      context->SetCursor(SplitVCursor);
      break;
    }

    case OnTopLeftCorner:
    case OnBottomRightCorner: {
      context->PushCursor();
      context->SetCursor(SizeFDiagCursor);
      break;
    }

    case OnTopRightCorner:
    case OnBottomLeftCorner: {
      context->PushCursor();
      context->SetCursor(SizeBDiagCursor);
      break;
    }

    default:
      break;
  }
}

}
