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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <blendint/opengl/gl-framebuffer.hpp>

#include <blendint/gui/flow-layout.hpp>
#include <blendint/gui/frame.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

Frame::Frame (AbstractLayout* layout)
    : AbstractFrame(),
      focused_widget_(0),
      hovered_widget_(0),
      cursor_position_(0),
      layout_(0),
      focused_(false),
      hover_(false),
      pressed_(false)
{
  if (layout == nullptr) {
    layout_ = new FlowLayout;
  } else {
    layout_ = layout;
  }

  PushBackSubView(layout_);
  set_size(layout_->size());

  InitializeFrameOnce();
}

Frame::Frame (int width, int height, AbstractLayout* layout)
    : AbstractFrame(width, height),
      focused_widget_(0),
      hovered_widget_(0),
      cursor_position_(0),
      layout_(0),
      focused_(false),
      hover_(false),
      pressed_(false)
{
  if (layout == nullptr) {
    layout_ = new FlowLayout;
  } else {
    layout_ = layout;
  }

  PushBackSubView(layout_);
  ResizeSubView(layout_, size());

  InitializeFrameOnce();
}

Frame::~Frame ()
{
  glDeleteVertexArrays(2, vao_);

  if (focused_widget_) {
    focused_widget_->destroyed().disconnect1(
        this, &Frame::OnFocusedWidgetDestroyed);
    focused_widget_ = 0;
  }

  if (hovered_widget_) {
    hovered_widget_->destroyed().disconnect1(
        this, &Frame::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, AbstractWindow::GetWindow(this));
  }
}

AbstractWidget* Frame::AddWidget (AbstractWidget* widget)
{
  return layout_->AddWidget(widget);
}

bool Frame::IsExpandX () const
{
  return false;
  //return layout_->IsExpandX();
}

bool Frame::IsExpandY () const
{
  return false;
  //return layout_->IsExpandY();
}

Size Frame::GetPreferredSize () const
{
  return layout_->GetPreferredSize();
}

void Frame::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if (target == this) {

    projection_matrix_ = glm::ortho(0.f,
                                    pixel_size(width),
                                    0.f,
                                    pixel_size(height),
                                    100.f,
                                    -100.f);

    set_size(width, height);

    if (view_buffer()) {
      view_buffer()->Resize(size());
    }

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;
    GenerateVertices(size(), 1.f,
                     RoundNone, 0.f, &inner_verts, &outer_verts);

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                      &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
                      &outer_verts[0]);
    vbo_.reset();

    ResizeSubView(layout_, size());
    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

bool Frame::PreDraw (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  if (refresh() && view_buffer()) {
    RenderToTexture(this, context, projection_matrix_, model_matrix_,
                             view_buffer()->texture());
  }

  return true;
}

Response Frame::Draw (AbstractWindow* context)
{
  AbstractWindow::shaders()->frame_inner_program()->use();

  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::FRAME_INNER_POSITION),
      pixel_size(position().x()), pixel_size(position().y()));
  glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_GAMMA),
              0);
  glUniform4f(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_COLOR),
              0.447f, 0.447f, 0.447f, 1.f);

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  if (view_buffer()) {

    AbstractWindow::shaders()->frame_image_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_POSITION),
        pixel_size(position().x()), pixel_size(position().y()));
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_GAMMA), 0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    view_buffer()->Draw(0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  } else {

    glViewport(pixel_size(position().x()),
               pixel_size(position().y()),
               pixel_size(size().width()),
               pixel_size(size().height()));

    AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
    AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

    DrawSubViewsOnce(context);

    glViewport(0, 0, pixel_size(context->size().width()),
               pixel_size(context->size().height()));

  }

  AbstractWindow::shaders()->frame_outer_program()->use();

  glUniform2f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_POSITION),
              pixel_size(position().x()),
              pixel_size(position().y()));
  glBindVertexArray(vao_[1]);

  glUniform4f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
              0.576f, 0.576f, 0.576f, 1.f);
  glDrawArrays(GL_TRIANGLE_STRIP, 4, 6);

  glUniform4f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
              0.4f, 0.4f, 0.4f, 1.f);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

  return Finish;
}

void Frame::PostDraw (AbstractWindow* context)
{
}

void Frame::PerformFocusOn (AbstractWindow* context)
{
  focused_ = true;
}

void Frame::PerformFocusOff (AbstractWindow* context)
{
  focused_ = false;

  if (hovered_widget_) {
    hovered_widget_->destroyed().disconnect1(
        this, &Frame::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, context);
    hovered_widget_ = 0;
  }

  if (focused_widget_) {
    focused_widget_->destroyed().disconnect1(
        this, &Frame::OnFocusedWidgetDestroyed);
    dispatch_focus_off(focused_widget_, context);
    focused_widget_ = 0;
  }
}

void Frame::PerformHoverIn (AbstractWindow* context)
{
  hover_ = true;
}

void Frame::PerformHoverOut (AbstractWindow* context)
{
  hover_ = false;

  if (hovered_widget_) {
    hovered_widget_->destroyed().disconnect1(
        this, &Frame::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, context);
    hovered_widget_ = 0;
  }
}

Response Frame::PerformKeyPress (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  Response response = Ignore;

  if (focused_widget_) {
    dispatch_key_press(focused_widget_, context);
  }

  return response;
}

Response Frame::PerformMousePress (AbstractWindow* context)
{
  if (cursor_position_ == InsideRectangle) {

    if (context == super()) context->SetFocusedFrame(this);

    DeclareActiveFrame(context, this);
    if (hovered_widget_) {

      AbstractView* widget = 0;	// widget may be focused

      widget = RecursiveDispatchMousePress(hovered_widget_, context);

      if (widget)
        SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);

    } else {
      // SetFocusedWidget(0);
    }

    pressed_ = true;
    return Finish;

  } else {
    pressed_ = false;
    return Ignore;
  }
}

Response Frame::PerformMouseRelease (AbstractWindow* context)
{
  // cursor_position_ = InsideRectangle;

  pressed_ = false;

  if (focused_widget_) {
    DeclareActiveFrame(context, this);
    return dispatch_mouse_release(focused_widget_, context);
  }

  return Ignore;
}

Response Frame::PerformMouseMove (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  Response retval = Ignore;
  if (pressed_) {
    if (focused_widget_) {
      retval = dispatch_mouse_move(focused_widget_, context);
    }
  } else {
    if(context->mouse_tracking()) {
      if (focused_widget_) {
        retval = dispatch_mouse_move(focused_widget_, context);
      }
    }
  }

  return retval;
}

Response Frame::PerformMouseHover (AbstractWindow* context)
{
  Response retval = Finish;

  if (Contain(context->GetGlobalCursorPosition())) {

    cursor_position_ = InsideRectangle;

    if (context->overlap()) {

      if (hover_) {
        DeclareActiveFrame(context, this);
        hover_ = false;
        if (hovered_widget_) {
          hovered_widget_->destroyed().disconnect1(
              this, &Frame::OnHoverWidgetDestroyed);
          ClearHoverWidgets(hovered_widget_, context);
          hovered_widget_ = 0;
        }
      }

      retval = Finish;

    } else {

      DeclareActiveFrame(context, this);

      if (!hover_) {
        PerformHoverIn(context);
      }

      AbstractWidget* new_hovered_widget = DispatchMouseHover(hovered_widget_,
                                                              context);

      if (new_hovered_widget != hovered_widget_) {

        if (hovered_widget_) {
          hovered_widget_->destroyed().disconnect1(
              this, &Frame::OnHoverWidgetDestroyed);
        }

        hovered_widget_ = new_hovered_widget;
        if (hovered_widget_) {
          hovered_widget_->destroyed().connect(this,
                                               &Frame::OnHoverWidgetDestroyed);
        }

      }

      context->set_overlap(true);
      retval = Ignore;
    }

  } else {

    cursor_position_ = OutsideRectangle;

    if (hover_) {
      DeclareActiveFrame(context, this);
      PerformHoverOut(context);
    }

    if (pressed_) {
      retval = Finish;
    } else {
      retval = Ignore;
    }
  }

  return retval;
}

void Frame::InitializeFrameOnce ()
{
  projection_matrix_ = glm::ortho(0.f,
                                  pixel_size(size().width()),
                                  0.f,
                                  pixel_size(size().height()),
                                  100.f,
                                  -100.f);
  model_matrix_ = glm::mat3(1.f);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;
  GenerateVertices(size(), 1.f, RoundNone, 0.f, &inner_verts,
                   &outer_verts);

  vbo_.generate();
  glGenVertexArrays(2, vao_);

  glBindVertexArray(vao_[0]);
  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

void Frame::SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context)
{
  if (focused_widget_ == widget) return;

  if (focused_widget_) {
    dispatch_focus_off(focused_widget_, context);
    focused_widget_->destroyed().disconnect1(
        this, &Frame::OnFocusedWidgetDestroyed);
  }

  focused_widget_ = widget;
  if (focused_widget_) {
    dispatch_focus_on(focused_widget_, context);
    focused_widget_->destroyed().connect(this,
                                         &Frame::OnFocusedWidgetDestroyed);
  }
}

void Frame::OnFocusedWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(focused_widget_ == widget);

  //set_widget_focus_status(widget, false);
  DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
  widget->destroyed().disconnect1(this, &Frame::OnFocusedWidgetDestroyed);

  focused_widget_ = 0;
}

void Frame::OnHoverWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(hovered_widget_ == widget);

  DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
  widget->destroyed().disconnect1(this, &Frame::OnHoverWidgetDestroyed);

  hovered_widget_ = 0;
}

}
