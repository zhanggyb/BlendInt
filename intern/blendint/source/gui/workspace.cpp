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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <opengl/opengl.hpp>

#include <gui/workspace.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  EdgeButton::EdgeButton (int round_type)
  : AbstractButton()
  {
    set_size(14, 14);
    set_round_type(round_type);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(&inner_verts, &outer_verts);

    glGenVertexArrays(2, vao_);
    glBindVertexArray(vao_[0]);

    buffer_.generate();

    buffer_.bind(0);
    buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3,
    GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(vao_[1]);
    buffer_.bind(1);
    buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2,
    GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(0);
    buffer_.reset();
  }

  EdgeButton::~EdgeButton ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void EdgeButton::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      GenerateRoundedVertices(&inner_verts, &outer_verts);

      buffer_.bind(0);
      buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                           &inner_verts[0]);
      buffer_.bind(1);
      buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
                           &outer_verts[0]);
      buffer_.reset();

      set_size(*request.size());

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  Response EdgeButton::Draw (AbstractWindow* context)
  {
    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
        0);

    if (is_checked()) {
      glUniform4fv(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
          AbstractWindow::theme()->radio_button().inner_sel.data());
    } else {
      glUniform4fv(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
          AbstractWindow::theme()->radio_button().inner.data());
    }

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->radio_button().outline.data());
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
        0.f, 0.f);

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 outline_vertex_count(round_type()) * 2 + 2);

    if (emboss()) {
      glUniform4f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
          1.0f, 1.0f, 1.0f, 0.16f);
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
          0.f, -1.f);
      glDrawArrays(GL_TRIANGLE_STRIP, 0,
                   emboss_vertex_count(round_type()) * 2);
    }

    glBindVertexArray(0);
    GLSLProgram::reset();

    return Finish;
  }

  // -------------------------------

  Workspace::Workspace ()
  : AbstractFrame(),
    left_frame_(0),
    right_frame_(0),
    header_frame_(0),
    main_frame_(0),
    splitter_(0),
    hover_frame_(0),
    focused_frame_(0),
    focused_(false),
    pressed_(false)
  {
    set_size(800, 600);

    InitializeWorkspace();
  }

  Workspace::~Workspace ()
  {
  }

  void Workspace::SetMainFrame (AbstractFrame* frame)
  {
    if ((frame == nullptr) || (frame == main_frame_)) return;

    if (main_frame_) {
      delete main_frame_;
    }

    if (left_frame_) {
      splitter_->InsertFrame(1, frame);
    } else {
      splitter_->InsertFrame(0, frame);
    }

    main_frame_ = frame;

    DBG_PRINT_MSG("viewport size: %d %d", main_frame_->size().width(),
                  main_frame_->size().height());
    RequestRedraw();
  }

  void Workspace::SetLeftFrame (Frame* frame)
  {
    if ((frame == nullptr) || (frame == left_frame_)) return;

    if (left_frame_) {
      delete left_frame_;
    }

    splitter_->InsertFrame(0, frame, PreferredWidth);
    left_frame_ = frame;
    RequestRedraw();
  }

  void Workspace::SetRightFrame (Frame* rightbar)
  {
    if ((rightbar == nullptr) || (rightbar == right_frame_)) return;

    if (right_frame_) {
      delete right_frame_;
    }

    splitter_->AddFrame(rightbar, PreferredWidth);
    right_frame_ = rightbar;
    RequestRedraw();
  }

  void Workspace::SetHeaderFrame (Frame* header, bool append)
  {
    if ((header == nullptr) || (header_frame_ == header)) return;

    if (header_frame_) {
      delete header_frame_;
    }

    header_frame_ = header;

    if (append) {

      if (PushBackSubView(header_frame_)) {
        Size prefer = header_frame_->GetPreferredSize();
        ResizeSubView(header_frame_, size().width(), prefer.height());
        ResizeSubView(splitter_, size().width(),
                      size().height() - prefer.height());

        MoveSubViewTo(header_frame_, position());
        MoveSubViewTo(splitter_, position().x(),
                      position().y() + header_frame_->size().height());
      } else {
        DBG_PRINT_MSG("Error: %s", "cannot add header frame");
      }

    } else {

      if (PushFrontSubView(header_frame_)) {
        Size prefer = header_frame_->GetPreferredSize();
        ResizeSubView(header_frame_, size().width(), prefer.height());
        ResizeSubView(splitter_, size().width(),
                      size().height() - prefer.height());

        MoveSubViewTo(splitter_, position());
        MoveSubViewTo(header_frame_, position().x(),
                      position().y() + splitter_->size().height());
      } else {
        DBG_PRINT_MSG("Error: %s", "cannot add header frame");
      }

    }

    RequestRedraw();
  }

  void Workspace::SwitchHeaderPosition ()
  {
    if (header_frame_) {

      if (first_subview() == header_frame_) {

        MoveToLast(header_frame_);

        MoveSubViewTo(header_frame_, position());
        MoveSubViewTo(splitter_, position().x(),
                      position().y() + header_frame_->size().height());

      } else {

        MoveToFirst(header_frame_);

        MoveSubViewTo(splitter_, position());
        MoveSubViewTo(header_frame_, position().x(),
                      position().y() + splitter_->size().height());

      }

      RequestRedraw();
    }
  }

  bool Workspace::IsExpandX () const
  {
    for (AbstractView* p = first_subview(); p; p = p->next_view()) {
      if (p->IsExpandX()) return true;
    }

    return false;
  }

  bool Workspace::IsExpandY () const
  {
    for (AbstractView* p = first_subview(); p; p = p->next_view()) {
      if (p->IsExpandY()) return true;
    }

    return false;
  }

  Size Workspace::GetPreferredSize () const
  {
    Size prefer;

    if (subs_count() == 0) {
      prefer.reset(500, 400);
    } else {
      Size tmp;
      for (AbstractView* p = first_subview(); p; p = p->next_view()) {
        tmp = p->GetPreferredSize();
        prefer.set_width(std::max(prefer.width(), tmp.width()));
        prefer.set_height(prefer.height() + tmp.height());
      }
    }

    return prefer;
  }

  void Workspace::PerformPositionUpdate (const PositionUpdateRequest& request)
  {
    if (request.target() == this) {
      int ox = request.position()->x() - position().x();
      int oy = request.position()->y() - position().y();

      set_position(*request.position());

      for (AbstractView* p = first_subview(); p; p = p->next_view()) {
        MoveSubViewTo(p, p->position().x() + ox, p->position().y() + oy);
      }

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportPositionUpdate(request);
    }
  }

  void Workspace::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {
      set_size(*request.size());

      if (header_frame_) {

        Size prefer = header_frame_->GetPreferredSize();

        ResizeSubView(header_frame_, size().width(), prefer.height());
        ResizeSubView(splitter_, size().width(),
                      size().height() - header_frame_->size().height());

        if (first_subview() == header_frame_) {
          MoveSubViewTo(splitter_, position());
          MoveSubViewTo(header_frame_, position().x(),
                        position().y() + splitter_->size().height());
        } else {
          MoveSubViewTo(header_frame_, position());
          MoveSubViewTo(splitter_, position().x(),
                        position().y() + header_frame_->size().height());
        }

      } else {
        ResizeSubView(splitter_, size());
      }

      RequestRedraw();
    }
    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  bool Workspace::PreDraw (AbstractWindow* context)
  {
    return visiable();
  }

  Response Workspace::Draw (AbstractWindow* context)
  {
    DrawSubViewsOnce(context);

    return subs_count() ? Ignore : Finish;
  }

  void Workspace::PostDraw (AbstractWindow* context)
  {
  }

  void Workspace::PerformFocusOn (AbstractWindow* context)
  {
    focused_ = true;
  }

  void Workspace::PerformFocusOff (AbstractWindow* context)
  {
    focused_ = false;

    if (focused_frame_) {
      dispatch_focus_off(focused_frame_, context);
      focused_frame_ = 0;
    }
  }

  void Workspace::PerformHoverIn (AbstractWindow* context)
  {
  }

  void Workspace::PerformHoverOut (AbstractWindow* context)
  {
    if (hover_frame_) {
      dispatch_mouse_hover_out(hover_frame_, context);
      hover_frame_ = nullptr;
    }
  }

  Response Workspace::PerformKeyPress (AbstractWindow* context)
  {
    if (focused_frame_) {
      return dispatch_key_press(focused_frame_, context);
    }
    return Ignore;
  }

  Response Workspace::PerformMousePress (AbstractWindow* context)
  {
    Response response = Ignore;

    pressed_ = true;

    if(context == superview()) context->SetFocusedFrame(this);

    if (hover_frame_ != nullptr) {
      response = dispatch_mouse_press(hover_frame_, context);

      if (response == Finish) {
        SetFocusedFrame(hover_frame_, context);
      }
    } else {
      SetFocusedFrame(0, context);
    }

    return Finish;
  }

  Response Workspace::PerformMouseRelease (AbstractWindow* context)
  {
    Response response = Ignore;

    pressed_ = false;

    if (focused_frame_ != nullptr) {
      response = dispatch_mouse_release(focused_frame_, context);
    }

    return response;
  }

  Response Workspace::PerformMouseMove (AbstractWindow* context)
  {
    Response response = Ignore;

    if (pressed_) {
      if (focused_frame_)
        response = dispatch_mouse_move(focused_frame_, context);
    } else {
      if (context->mouse_tracking() && focused_frame_) response =
          dispatch_mouse_move(focused_frame_, context);
    }

    return response;
  }

  Response Workspace::PerformMouseHover (AbstractWindow* context)
  {
    if (Contain(context->GetGlobalCursorPosition())) {

      Response response = Finish;
      SetHoveredFrame(context);
      if (hover_frame_ != nullptr) {
        response = dispatch_mouse_hover(hover_frame_, context);
      }

      return response;

    } else {
      return Ignore;
    }
  }

  void Workspace::InitializeWorkspace ()
  {
    splitter_ = Manage(new FrameSplitter);
    DBG_SET_NAME(splitter_, "Splitter");

    PushBackSubView(splitter_);
    ResizeSubView(splitter_, size());
    MoveSubViewTo(splitter_, position());
  }

  void Workspace::SetFocusedFrame (AbstractFrame* frame,
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

  bool Workspace::RemoveSubView (AbstractView* view)
  {
    if (view == left_frame_) {
      left_frame_ = nullptr;
    } else if (view == right_frame_) {
      right_frame_ = nullptr;
    } else if (view == header_frame_) {
      header_frame_ = nullptr;
    } else if (view == main_frame_) {
      main_frame_ = nullptr;
    }

    if (view == hover_frame_) {
      hover_frame_ = nullptr;
    }

    if (view == focused_frame_) {
      focused_frame_ = nullptr;
    }

    return AbstractFrame::RemoveSubView(view);
  }

  void Workspace::SetHoveredFrame (AbstractWindow* context)
  {
    AbstractFrame* original = hover_frame_;

    if (hover_frame_ != nullptr) {
      if (!hover_frame_->Contain(context->GetGlobalCursorPosition())) {

        hover_frame_ = nullptr;
        for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
          if (p->Contain(context->GetGlobalCursorPosition())) {
            hover_frame_ = dynamic_cast<AbstractFrame*>(p);
            break;
          }
        }

      }
    } else {

      for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
        if (p->Contain(context->GetGlobalCursorPosition())) {
          hover_frame_ = dynamic_cast<AbstractFrame*>(p);
          break;
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
