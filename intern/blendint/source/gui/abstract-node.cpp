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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <gui/abstract-node.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  void AbstractNode::SetRoundRadius (float radius)
  {
    if (round_radius_ == radius) return;

    PerformRoundRadiusUpdate(radius);
  }

  void AbstractNode::SetRoundType (int type)
  {
    if ((node_flag_ & 0x0F) == (type & 0x0F)) return;

    PerformRoundTypeUpdate(type & 0x0F);
  }

  Point AbstractNode::GetRelativePosition (const AbstractWidget* widget)
  {
    if (widget == nullptr)
      throw std::invalid_argument("Argument cannot be a nullptr!");

    Point pos = widget->position();
    AbstractNode* node = 0;
    AbstractView* p = widget->superview();

    while (p) {

      node = dynamic_cast<AbstractNode*>(p);
      if (node) break;

      pos = pos + p->position() + p->GetOffset();
      p = p->superview();

    }

    if (node != this)
      throw std::out_of_range("Widget is not contained in this node!");

    return pos;
  }

  void AbstractNode::GenerateRoundedVertices (std::vector<GLfloat>* inner,
                                              std::vector<GLfloat>* outer)
  {
    GenerateVertices(size(),
                     default_border_width() * AbstractWindow::theme()->pixel(),
                     round_type(), round_radius_, inner, outer);
  }

  void AbstractNode::GenerateRoundedVertices (Orientation shadedir,
                                              short shadetop,
                                              short shadedown,
                                              std::vector<GLfloat>* inner,
                                              std::vector<GLfloat>* outer)
  {
    GenerateVertices(size(),
                     default_border_width() * AbstractWindow::theme()->pixel(),
                     round_type(), round_radius_, shadedir, shadetop, shadedown,
                     inner, outer);
  }

  void AbstractNode::PerformRoundTypeUpdate (int round)
  {
    set_round_type(round);
  }

  bool AbstractNode::PreDraw (AbstractWindow* context)
  {
    if (!visiable()) return false;

    Point offset = GetOffset();

    glm::mat3 matrix = glm::translate(
        AbstractWindow::shaders()->widget_model_matrix(),
        glm::vec2(position().x() + offset.x(), position().y() + offset.y()));

    AbstractWindow::shaders()->PushWidgetModelMatrix();
    AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

    return true;
  }

  void AbstractNode::PostDraw (AbstractWindow* context)
  {
    AbstractWindow::shaders()->PopWidgetModelMatrix();
  }

  void AbstractNode::PerformFocusOn (AbstractWindow* context)
  {
  }

  void AbstractNode::PerformFocusOff (AbstractWindow* context)
  {
  }

  void AbstractNode::PerformHoverIn (AbstractWindow* context)
  {
  }

  void AbstractNode::PerformHoverOut (AbstractWindow* context)
  {
  }

  Response AbstractNode::PerformKeyPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractNode::PerformContextMenuPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractNode::PerformContextMenuRelease (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractNode::PerformMousePress (AbstractWindow* context)
  {
    if (cursor_position_ == InsideRectangle) {

      last_position_ = position();
      cursor_point_ = context->local_cursor_position();

      /*
       if(hovered_widget_) {

       AbstractView* widget = 0;	// widget may be focused

       widget = DispatchMousePressEvent(hovered_widget_, context);

       if(widget == 0) {
       //DBG_PRINT_MSG("%s", "widget 0");
       set_mouse_button_pressed(true);
       } else {
       SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
       }

       } else {
       set_mouse_button_pressed(true);
       }
       */
      set_mouse_button_pressed(true);

      return Finish;

    } else if (cursor_position_ != OutsideRectangle) {

      set_mouse_button_pressed(true);

      last_position_ = position();
      last_size_ = size();
      cursor_point_ = context->local_cursor_position();

      return Finish;
    }

    return Ignore;
  }

  Response AbstractNode::PerformMouseRelease (AbstractWindow* context)
  {
    cursor_position_ = InsideRectangle;
    set_mouse_button_pressed(false);

//		if(focused_widget_) {
//			context->register_active_frame(this);
//			return dispatch_mouse_release(focused_widget_, context);
//		}

    return Ignore;
  }

  Response AbstractNode::PerformMouseMove (AbstractWindow* context)
  {
    Response retval = Ignore;

    if (mouse_button_pressed()) {

      int ox = context->local_cursor_position().x() - cursor_point_.x();
      int oy = context->local_cursor_position().y() - cursor_point_.y();

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

      if (superview()) {
        superview()->RequestRedraw();
      }
      retval = Finish;

    } else {

//			if(focused_widget_) {
//
//				context->register_active_frame(this);
//				retval = dispatch_mouse_move(focused_widget_, context);
//
//			}
    }

    return retval;
  }

  AbstractNode::AbstractNode (int flag)
      :
        AbstractView(),
        node_flag_(flag),
        round_radius_(5.f),
        cursor_position_(InsideRectangle)
  {
  }

  AbstractNode::AbstractNode (int width, int height, int flag)
      :
        AbstractView(width, height),
        node_flag_(flag),
        round_radius_(5.f),
        cursor_position_(InsideRectangle)
  {

  }

  AbstractNode::~AbstractNode ()
  {
  }

  AbstractWidget* AbstractNode::DispatchMouseHover (AbstractWidget* orig,
                                                    AbstractWindow* context)
  {
    // find the new top hovered widget
    if (orig != nullptr) {

      if (orig->superview() == 0) { // the widget is just removed from this frame
        return FindAndDispatchTopHoveredWidget(context);
      }

      if (orig->superview() == this) {
        return RecheckAndDispatchTopHoveredWidget(orig, context);
      }

      Rect rect;
      try {
        rect.set_position(
            GetRelativePosition(
                dynamic_cast<AbstractWidget*>(orig->superview())));
        rect.set_size(orig->superview()->size());
        return RecheckAndDispatchTopHoveredWidget(rect, orig, context);

      }

      catch (const std::bad_cast& e) {
        DBG_PRINT_MSG("Error: %s", e.what());
        return FindAndDispatchTopHoveredWidget(context);
      }

      catch (const std::invalid_argument& e) {
        DBG_PRINT_MSG("Error: %s", e.what());
        return FindAndDispatchTopHoveredWidget(context);
      }

      catch (const std::out_of_range& e) {
        DBG_PRINT_MSG("Error: %s", e.what());
        return FindAndDispatchTopHoveredWidget(context);
      }

    } else {

      return FindAndDispatchTopHoveredWidget(context);

    }
  }

  void AbstractNode::PerformRoundRadiusUpdate (float radius)
  {
    round_radius_ = radius;
  }

  Response AbstractNode::PerformMouseHover (AbstractWindow* context)
  {
    if (mouse_button_pressed()) return Finish;

    Response retval = Finish;
    int border = 4;

    // Point global_position = context->GetAbsolutePosition(this);

    Rect valid_rect(position().x() - border, position().y() - border,
                    size().width() + 2 * border, size().height() + 2 * border);

    if (valid_rect.contains(context->local_cursor_position())) {

      if (Contain(context->local_cursor_position())) {

        cursor_position_ = InsideRectangle;

        // DBG_PRINT_MSG("Cursor position: (%d, %d)", context->GetGlobalCursorPosition().x(), context->GetGlobalCursorPosition().y());

        /*
         AbstractWidget* new_hovered_widget = DispatchHoverEventsInWidgets(hovered_widget_, context);

         if(new_hovered_widget != hovered_widget_) {

         if(hovered_widget_) {
         hovered_widget_->destroyed().disconnectOne(this,
         &AbstractDialog::OnHoverWidgetDestroyed);
         }

         hovered_widget_ = new_hovered_widget;
         if(hovered_widget_) {
         events()->connect(hovered_widget_->destroyed(), this,
         &AbstractDialog::OnHoverWidgetDestroyed);
         }

         }
         */

//				if(hovered_widget_) {
//					 DBG_PRINT_MSG("hovered widget: %s", hovered_widget_->name().c_str());
//				}
// set cursor shape
        if (cursor_on_border()) {
          set_cursor_on_border(false);
          context->PopCursor();
        }

      } else {

        set_cursor_on_border(true);
        cursor_position_ = InsideRectangle;

        if (context->local_cursor_position().x() <= position().x()) {
          cursor_position_ |= OnLeftBorder;
        } else if (context->local_cursor_position().x()
            >= (position().x() + size().width())) {
          cursor_position_ |= OnRightBorder;
        }

        if (context->local_cursor_position().y()
            >= (position().y() + size().height())) {
          cursor_position_ |= OnTopBorder;
        } else if (context->local_cursor_position().y() <= position().y()) {
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

    } else {
      cursor_position_ = OutsideRectangle;

      // set cursor shape
      if (cursor_on_border()) {
        set_cursor_on_border(false);
        context->PopCursor();
      }

      retval = Ignore;
    }

    return retval;
  }

  AbstractWidget* AbstractNode::RecheckAndDispatchTopHoveredWidget (AbstractWidget* orig,
                                                                    AbstractWindow* context)
  {
    assert(orig->superview() == this);

    AbstractWidget* result = orig;
    Point offset;
    Rect rect;

    rect.set_position(position());
    rect.set_size(size());

    bool cursor_in_node = rect.contains(context->local_cursor_position());

    if (cursor_in_node) {

      offset = GetOffset();
      context->set_local_cursor_position(
          context->local_cursor_position().x() - rect.x() - offset.x(),
          context->local_cursor_position().y() - rect.y() - offset.y());

      if (result->Contain(context->local_cursor_position())) {
        result = RecursiveDispatchHoverEvent(result, context);
      } else {
        dispatch_mouse_hover_out(result, context);
        result = FindAndDispatchTopHoveredWidget(context);
      }

    } else {

      dispatch_mouse_hover_out(result, context);
      result = 0;

    }

    return result;
  }

  AbstractWidget* AbstractNode::RecheckAndDispatchTopHoveredWidget (Rect& rect,
                                                                    AbstractWidget* orig,
                                                                    AbstractWindow* context)
  {
    assert(orig);
    assert(orig->superview() && orig->superview() != this);

    AbstractWidget* result = orig;
    AbstractView* super_view = result->superview();
    AbstractWidget* super_widget = dynamic_cast<AbstractWidget*>(super_view);
    Point offset;

    bool cursor_in_superview = rect.contains(
        context->local_cursor_position());

    if (cursor_in_superview) {

      offset = super_view->GetOffset();
      context->set_local_cursor_position(
          context->local_cursor_position().x() - rect.x() - offset.x(),
          context->local_cursor_position().y() - rect.y() - offset.y());

      if (result->Contain(context->local_cursor_position())) {
        result = RecursiveDispatchHoverEvent(result, context);
      } else {

        dispatch_mouse_hover_out(result, context);

        // find which contianer contains cursor position
        while (super_view) {

          if (super_view == this) {
            super_view = 0;
            break;
          }

          offset = super_view->GetOffset();
          context->set_local_cursor_position(
              context->local_cursor_position().x() + super_view->position().x()
                  + offset.x(),
              context->local_cursor_position().y() + super_view->position().y()
                  + offset.y());

          if (super_view->Contain(context->local_cursor_position())) break;

          super_view = super_view->superview();
        }

        result = dynamic_cast<AbstractWidget*>(super_view);

        if (result) {
          result = RecursiveDispatchHoverEvent(result, context);
        }

      }

    } else {

      dispatch_mouse_hover_out(result, context);

      // find which contianer contains cursor position
      super_view = super_view->superview();
      while (super_view != nullptr) {

        if (super_view == this) {
          super_view = nullptr;
          break;
        }

        super_widget = dynamic_cast<AbstractWidget*>(super_view);
        if (super_widget) {
          rect.set_position(GetRelativePosition(super_widget));
          rect.set_size(super_widget->size());
        } else {
          assert(super_view == this);
          rect.set_position(position());
          rect.set_size(size());
        }

        offset = super_view->GetOffset();
        context->set_local_cursor_position(
            context->local_cursor_position().x() - rect.x() - offset.x(),
            context->local_cursor_position().y() - rect.y() - offset.y());

        if (rect.contains(context->local_cursor_position())) break;

        super_view = super_view->superview();
      }

      result = dynamic_cast<AbstractWidget*>(super_view);
      if (result) {

        AbstractWidget* tmp = 0;
        for (AbstractView* p = super_widget->last_subview(); p;
            p = p->previous_view()) {

          tmp = dynamic_cast<AbstractWidget*>(p);
          if (tmp) {
            if (p->visiable() && p->Contain(context->local_cursor_position())) {
              result = tmp;

              dispatch_mouse_hover_in(result, context);
              result = RecursiveDispatchHoverEvent(result, context);

              break;
            }
          } else {
            break;
          }
        }
      }

    }

    return result;
  }

  AbstractWidget* AbstractNode::FindAndDispatchTopHoveredWidget (AbstractWindow* context)
  {
    AbstractWidget* result = 0;
    Point offset = GetOffset();

    context->set_local_cursor_position(
        context->local_cursor_position().x() - position().x() - offset.x(),
        context->local_cursor_position().y() - position().y() - offset.y());

    for (AbstractView* p = last_subview(); p; p = p->previous_view()) {

      result = dynamic_cast<AbstractWidget*>(p);
      if (result) {
        if (p->visiable() && p->Contain(context->local_cursor_position())) {
          dispatch_mouse_hover_in(result, context);
          break;
        }
      } else {
        break;
      }

    }

    if (result) {
      result = RecursiveDispatchHoverEvent(result, context);
    }

    return result;
  }

  AbstractWidget* AbstractNode::RecursiveDispatchHoverEvent (AbstractWidget* widget,
                                                             AbstractWindow* context)
  {
    AbstractWidget* retval = widget;
    AbstractWidget* tmp = 0;

    Point offset = widget->GetOffset();
    context->set_local_cursor_position(
        context->local_cursor_position().x() - widget->position().x()
            - offset.x(),
        context->local_cursor_position().y() - widget->position().y()
            - offset.y());

    for (AbstractView* p = widget->last_subview(); p; p = p->previous_view()) {

      tmp = dynamic_cast<AbstractWidget*>(p);

      if (tmp) {
        if (p->visiable() && p->Contain(context->local_cursor_position())) {
          retval = tmp;
          dispatch_mouse_hover_in(retval, context);
          retval = RecursiveDispatchHoverEvent(retval, context);

          break;
        }
      } else {
        break;
      }

    }

    return retval;
  }

}
