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

#include <blendint/gui/abstract-node.hpp>
#include <blendint/gui/node-view.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

  AbstractNode::AbstractNode (int flag)
  : AbstractView(),
    node_flag_(flag),
    round_radius_(5.f),
    focused_widget_(0),
    hovered_widget_(0),
    cursor_position_(InsideRectangle),
    focused_(false),
    pressed_(false)
  {
    set_view_type(ViewTypeNode);
  }

  AbstractNode::AbstractNode (int width, int height, int flag)
  : AbstractView(width, height),
    node_flag_(flag),
    round_radius_(5.f),
    focused_widget_(0),
    hovered_widget_(0),
    cursor_position_(InsideRectangle),
    focused_(false),
    pressed_(false)
  {
    set_view_type(ViewTypeNode);
  }

  AbstractNode::~AbstractNode ()
  {
    destroyed_.Invoke(this);

    if (subview_count() > 0) {
      ClearSubViews();
    } else {
      DBG_ASSERT(subview_count_ == 0);
      DBG_ASSERT(first_ == 0);
      DBG_ASSERT(last_ == 0);
    }

    if (super_) {
      super_->RemoveSubView(this);
    } else {
      DBG_ASSERT(previous_ == 0);
      DBG_ASSERT(next_ == 0);
    }
  }

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

  Point AbstractNode::GetRelativePosition (const AbstractView* widget)
  {
    if (widget == nullptr)
      throw std::invalid_argument("Argument cannot be a nullptr!");

    Point pos = widget->position();
    AbstractNode* node = 0;
    AbstractView* parent = widget->super();

    while (parent) {

      if (is_node(parent)) {
        node = dynamic_cast<AbstractNode*>(parent);
        break;
      }

      pos = pos + parent->position() + parent->GetOffset();
      parent = parent->super();

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
    GenerateRawVertices(0.f,
                        0.f,
                        pixel_size(size().width()),
                        pixel_size(size().height()),
                        pixel_size(default_border_width()),
                        round_type(),
                        pixel_size(round_radius()),
                        shadedir,
                        shadetop,
                        shadedown,
                        inner,
                        outer);
  }

  void AbstractNode::PerformRoundTypeUpdate (int round)
  {
    set_round_type(round);
  }

  bool AbstractNode::PreDraw (AbstractWindow* context)
  {
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
    focused_ = true;
  }

  void AbstractNode::PerformFocusOff (AbstractWindow* context)
  {
    focused_ = false;
  }

  void AbstractNode::PerformHoverIn (AbstractWindow* context)
  {
  }

  void AbstractNode::PerformHoverOut (AbstractWindow* context)
  {
  }

  Response AbstractNode::PerformKeyPress (AbstractWindow* context)
  {
    Response response = Ignore;

    if (focused_widget_) {
      response = RecursiveDispatchKeyEvent(focused_widget_, context);
    }

    return response;
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
    NodeView* node_view = dynamic_cast<NodeView*>(super());

    if (cursor_position_ == InsideRectangle) {

      if (node_view) node_view->SetFocusedNode(this);

      last_position_ = position();
      cursor_point_ = context->GetGlobalCursorPosition();

      if (hovered_widget_) {

        AbstractView* widget = RecursiveDispatchMousePress(hovered_widget_,
                                                           context);

        if (widget == 0) {
          pressed_ = true;
        } else {
          SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
        }

      } else {
        pressed_ = true;
      }

      return Finish;

    } else if (cursor_position_ != OutsideRectangle) {

      if (node_view) node_view->SetFocusedNode(this);

      pressed_ = true;

      last_position_ = position();
      last_size_ = size();
      cursor_point_ = context->GetGlobalCursorPosition();

      return Finish;
    }

    return Ignore;
  }

  Response AbstractNode::PerformMouseRelease (AbstractWindow* context)
  {
    Response result = Ignore;

    if (pressed_) {
      pressed_ = false;
      result = Finish;
    }

    cursor_position_ = InsideRectangle;

    if (focused_widget_) {
      return dispatch_mouse_release(focused_widget_, context);
    }

    return result;
  }

  Response AbstractNode::PerformMouseMove (AbstractWindow* context)
  {
    Response retval = Ignore;

    if (pressed_) {

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

      if (focused_widget_)
        retval = dispatch_mouse_move(focused_widget_, context);

    }

    return retval;
  }

  AbstractWidget* AbstractNode::DispatchMouseHover (AbstractWidget* orig,
                                                    AbstractWindow* context)
  {
    // find the new top hovered widget

    if (orig == 0) return FindWidgetUnderCursor(context);

    if (orig->super() == 0) { // the widget is just removed from this frame
      return FindWidgetUnderCursor(context);
    }

    if (orig->super() == this) {
      return RecheckSubWidgetUnderCursor(orig, context);
    }

    try {
      return RecheckWidgetUnderCursor(orig, context);
    }

    catch (const std::bad_cast& e) {
      DBG_PRINT_MSG("Error: %s", e.what());
      return FindWidgetUnderCursor(context);
    }

    catch (const std::invalid_argument& e) {
      DBG_PRINT_MSG("Error: %s", e.what());
      return FindWidgetUnderCursor(context);
    }

    catch (const std::out_of_range& e) {
      DBG_PRINT_MSG("Error: %s", e.what());
      return FindWidgetUnderCursor(context);
    }
  }

  void AbstractNode::PerformRoundRadiusUpdate (float radius)
  {
    round_radius_ = radius;
  }

  Response AbstractNode::PerformMouseHover (AbstractWindow* context)
  {
    if (pressed_) return Finish;

    Response retval = Finish;
    int border = 4;

    Rect valid_rect(position().x() - border, position().y() - border,
                    size().width() + 2 * border, size().height() + 2 * border);

    if (valid_rect.contains(context->local_cursor_position())) {

      if (Contain(context->local_cursor_position())) {

        cursor_position_ = InsideRectangle;

        AbstractWidget* new_hovered_widget = DispatchMouseHover(hovered_widget_,
                                                                context);

        if (new_hovered_widget != hovered_widget_) {

          if (hovered_widget_) {
            hovered_widget_->destroyed().disconnect1(
                this, &AbstractNode::OnHoverWidgetDestroyed);
          }

          hovered_widget_ = new_hovered_widget;
          if (hovered_widget_) {
            hovered_widget_->destroyed().connect(this,
                              &AbstractNode::OnHoverWidgetDestroyed);
          }

        }

//				if(hovered_widget_) {
//					 DBG_PRINT_MSG("hovered widget: %s", hovered_widget_->name().c_str());
//				}

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

  void AbstractNode::SetFocusedWidget (AbstractWidget* widget,
                                       AbstractWindow* context)
  {
    if (focused_widget_ == widget) return;

    if (focused_widget_) {
      dispatch_focus_off(focused_widget_, context);
      focused_widget_->destroyed().disconnect1(
          this, &AbstractNode::OnFocusedWidgetDestroyed);
    }

    focused_widget_ = widget;
    if (focused_widget_) {
      dispatch_focus_on(focused_widget_, context);
      focused_widget_->destroyed().connect(this,
                        &AbstractNode::OnFocusedWidgetDestroyed);
    }
  }

  AbstractWidget* AbstractNode::RecheckSubWidgetUnderCursor (AbstractWidget* orig,
                                                             AbstractWindow* context)
  {
    DBG_ASSERT(orig->super() == this);

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
        result = FindWidgetUnderCursor(context);
      }

    } else {

      dispatch_mouse_hover_out(result, context);
      result = 0;

    }

    return result;
  }

  AbstractWidget* AbstractNode::RecheckWidgetUnderCursor (AbstractWidget* orig,
                                                          AbstractWindow* context)
  {
    DBG_ASSERT(orig);
    DBG_ASSERT(orig->super() && orig->super() != this);

    AbstractWidget* result = orig;
    AbstractView* parent = result->super();
    Point offset;

    Rect rect(GetRelativePosition(parent) + position() + GetOffset(),
              parent->size());

    bool cursor_in_superview = rect.contains(context->local_cursor_position());

    if (cursor_in_superview) {

      offset = parent->GetOffset();
      context->set_local_cursor_position(
          context->local_cursor_position().x() - rect.x() - offset.x(),
          context->local_cursor_position().y() - rect.y() - offset.y());

      if (result->Contain(context->local_cursor_position())) {
        result = RecursiveDispatchHoverEvent(result, context);
      } else {

        dispatch_mouse_hover_out(result, context);

        // find which contianer contains cursor position
        while (parent) {

          if (parent == this) {
            parent = 0;
            break;
          }

          offset = parent->GetOffset();
          context->set_local_cursor_position(
              context->local_cursor_position().x() + parent->position().x()
                  + offset.x(),
              context->local_cursor_position().y() + parent->position().y()
                  + offset.y());

          if (parent->Contain(context->local_cursor_position())) break;

          parent = parent->super();
        }

        result = dynamic_cast<AbstractWidget*>(parent);

        if (result) {
          result = RecursiveDispatchHoverEvent(result, context);
        }

      }

    } else {

      dispatch_mouse_hover_out(result, context);

      // find which contianer contains cursor position
      parent = parent->super();
      while (parent != nullptr) {

        if (parent == this) {
          parent = nullptr;
          break;
        }

        if (is_widget(parent)) {
          rect.set_position(GetRelativePosition(parent));
          rect.set_size(parent->size());
        } else {
          DBG_ASSERT(parent == this);
          rect.set_position(position());
          rect.set_size(size());
        }

        offset = parent->GetOffset();
        context->set_local_cursor_position(
            context->local_cursor_position().x() - rect.x() - offset.x(),
            context->local_cursor_position().y() - rect.y() - offset.y());

        if (rect.contains(context->local_cursor_position())) break;

        parent = parent->super();
      }

      result = dynamic_cast<AbstractWidget*>(parent);
      if (result) {

        for (AbstractView* p = parent->GetLastSubView(); p;
            p = parent->GetPreviousSubView(p)) {

          if (is_widget(p)) {
            if (p->Contain(context->local_cursor_position())) {
              result = dynamic_cast<AbstractWidget*>(p);

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

  AbstractWidget* AbstractNode::FindWidgetUnderCursor (AbstractWindow* context)
  {
    AbstractWidget* result = 0;
    Point offset = GetOffset();

    context->set_local_cursor_position(
        context->local_cursor_position().x() - position().x() - offset.x(),
        context->local_cursor_position().y() - position().y() - offset.y());

    for (AbstractView* p = GetLastSubView(); p; p = GetPreviousSubView(p)) {

      if (is_widget(p)) {
        result = dynamic_cast<AbstractWidget*>(p);
        if (p->Contain(context->local_cursor_position())) {
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
    Point offset = widget->GetOffset();
    context->set_local_cursor_position(
        context->local_cursor_position().x() - widget->position().x()
            - offset.x(),
        context->local_cursor_position().y() - widget->position().y()
            - offset.y());

    for (AbstractView* p = widget->GetLastSubView(); p;
        p = widget->GetPreviousSubView(p)) {

      if (is_widget(p)) {
        if (p->Contain(context->local_cursor_position())) {
          retval = dynamic_cast<AbstractWidget*>(p);
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

  void AbstractNode::OnFocusedWidgetDestroyed (AbstractWidget* widget)
  {
    DBG_ASSERT(focused_widget_ == widget);
    DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
    widget->destroyed().disconnect1(this,
                                      &AbstractNode::OnFocusedWidgetDestroyed);

    focused_widget_ = 0;
  }

  void AbstractNode::OnHoverWidgetDestroyed (AbstractWidget* widget)
  {
    DBG_ASSERT(hovered_widget_ == widget);
    DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
    widget->destroyed().disconnect1(this,
                                      &AbstractNode::OnHoverWidgetDestroyed);

    hovered_widget_ = 0;
  }

}
