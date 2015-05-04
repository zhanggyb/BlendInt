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

#include <blendint/gui/menu.hpp>

#include <blendint/gui/linear-adjustment.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

int Menu::kDefaultIconSpace = 4;
int Menu::kDefaultShortcutSpace = 20;

Menu::Menu ()
    :
      AbstractRoundFrame(FrameTopmost),
      focused_widget_(nullptr),
      hovered_widget_(nullptr),
      cursor_range_(0),
      space_(1),
      hover_(false),
      pressed_(false)
{
  set_size(240, 360);
  set_round_type(RoundAll);
  set_refresh(true);
  EnableViewBuffer();

  projection_matrix_ = glm::ortho(0.f, (float) size().width(), 0.f,
                                  (float) size().height(), 100.f, -100.f);
  model_matrix_ = glm::mat3(1.f);

  InitializeMenu();

  shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
}

Menu::~Menu ()
{
  glDeleteVertexArrays(2, vao_);

  if (focused_widget_) {
    focused_widget_->destroyed().disconnect1(this,
                                               &Menu::OnFocusedWidgetDestroyed);
    focused_widget_ = 0;
  }

  if (hovered_widget_) {
    hovered_widget_->destroyed().disconnect1(this,
                                               &Menu::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, AbstractWindow::GetWindow(this));
  }

}

void Menu::SetTitle (const String& title)
{
  title_ = title;
}

void Menu::AddAction (const String& text)
{
  MenuItem* item = new MenuItem(text);

  AddWidget(item);
}

void Menu::AddAction (const String& text, const String& shortcut)
{
  MenuItem* item = new MenuItem(text, shortcut);

  AddWidget(item);
}

void Menu::AddAction (const RefPtr<AbstractIcon>& icon, const String& text)
{
  MenuItem* item = new MenuItem(icon, text);

  AddWidget(item);
}

void Menu::AddAction (const RefPtr<AbstractIcon>& icon,
                      const String& text,
                      const String& shortcut)
{
  MenuItem* item = new MenuItem(icon, text, shortcut);

  AddWidget(item);
}

void Menu::AddAction (const RefPtr<Action>& action)
{
  if (action) {
    MenuItem* item = new MenuItem(action);
    AddWidget(item);
  }
}

bool Menu::AddWidget (AbstractWidget* item)
{
  if (PushBackSubView(item)) {

    int x = 0;
    int y = round_radius();
    int w = size().width();
    int h = size().height() - 2 * y;

    LinearAdjustment adjust(this, Vertical, AlignLeft, space_);
    adjust.Adjust(x, y, w, h);

    RequestRedraw();
    return true;
  }

  return false;
}

bool Menu::InsertWidget (int index, AbstractWidget* item)
{
  if (InsertWidget(index, item)) {

    int x = 0;
    int y = round_radius();
    int w = size().width();
    int h = size().height() - 2 * y;

    LinearAdjustment adjust(this, Vertical, AlignLeft, space_);
    adjust.Adjust(x, y, w, h);

    RequestRedraw();
    return true;
  }

  return false;
}

Size Menu::GetPreferredSize () const
{
  int radius_plus = round_radius() * 2;

  int w = 0;
  int h = 0;

  if (subview_count()) {

    Size tmp;

    for (AbstractView* p = first(); p; p = next(p)) {
      tmp = p->GetPreferredSize();
      w = std::max(w, tmp.width());
      h += tmp.height();
    }

    h += (subview_count() - 1) * space_;

  } else {
    w = 240;
  }

  h += radius_plus;

  return Size(w, h);
}

Response Menu::PerformMouseMove (AbstractWindow* context)
{
  return Finish;
}

Response Menu::PerformMousePress (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  if (cursor_range_ == InsideRectangle) {

    if (hovered_widget_) {

      AbstractView* widget = 0;	// widget may be focused

      widget = RecursiveDispatchMousePress(hovered_widget_, context);
      if (widget == 0) {
        DBG_PRINT_MSG("%s", "widget 0");
        pressed_ = true;
      } else {
        SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
      }

    } else {
      pressed_ = true;
    }

  } else if (cursor_range_ == OutsideRectangle) {
    pressed_ = false;
    delete this;
    return Finish;
  }

  return Finish;
}

Response Menu::PerformMouseRelease (AbstractWindow* context)
{
  cursor_range_ = InsideRectangle;

  pressed_ = false;

  if (focused_widget_) {
    DeclareActiveFrame(context, this);
    return dispatch_mouse_release(focused_widget_, context);
  }

  return Ignore;
}

void Menu::PerformSizeUpdate (const AbstractView* source,
                              const AbstractView* target,
                              int width,
                              int height)
{
  if (target == this) {

    set_size(width, height);

    projection_matrix_ = glm::ortho(0.f, 0.f + size().width(), 0.f,
                                    0.f + size().height(), 100.f, -100.f);

    if (view_buffer()) {
      view_buffer()->Resize(size());
    }

    shadow_->Resize(size());

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu_back().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->menu_back().shadetop,
                              AbstractWindow::theme()->menu_back().shadedown,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    int x = 0;
    int y = round_radius();
    int w = size().width();
    int h = size().height() - 2 * y;

    LinearAdjustment adjust(this, Vertical, AlignLeft, space_);
    adjust.Adjust(x, y, w, h);

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void Menu::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->menu_back().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->menu_back().shadetop,
                            AbstractWindow::theme()->menu_back().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  shadow_->SetRoundType(round_type);
}

void Menu::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->menu_back().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->menu_back().shadetop,
                            AbstractWindow::theme()->menu_back().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  shadow_->SetRadius(radius);
}

void Menu::PerformFocusOn (AbstractWindow* context)
{
}

void Menu::PerformFocusOff (AbstractWindow* context)
{
}

bool Menu::PreDraw (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  if (refresh() && view_buffer()) {
    RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_,
                             view_buffer()->texture());
  }

  return true;
}

Response Menu::Draw (AbstractWindow* context)
{
  shadow_->Draw(position().x(), position().y());

  AbstractWindow::shaders()->frame_inner_program()->use();

  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::FRAME_INNER_POSITION),
      (float) position().x(), (float) position().y());
  glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_GAMMA),
              0);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_COLOR),
               1, AbstractWindow::theme()->menu_back().inner.data());

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  if (view_buffer()) {

    AbstractWindow::shaders()->frame_image_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_POSITION),
        position().x(), position().y());
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
    glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_GAMMA),
                0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    view_buffer()->Draw(0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  } else {

    glViewport(position().x(), position().y(), size().width(), size().height());

    AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
    AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

    DrawSubViewsOnce(context);

    glViewport(0, 0, context->size().width(), context->size().height());

  }

  AbstractWindow::shaders()->frame_outer_program()->use();

  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_POSITION),
      position().x(), position().y());
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
               1, AbstractWindow::theme()->menu_back().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  return Finish;
}

void Menu::PostDraw (AbstractWindow* context)
{
}

void Menu::PerformHoverIn (AbstractWindow* context)
{
  hover_ = true;
}

void Menu::PerformHoverOut (AbstractWindow* context)
{
  hover_ = false;

  if (hovered_widget_) {
    hovered_widget_->destroyed().disconnect1(this,
                                               &Menu::OnHoverWidgetDestroyed);
    ClearHoverWidgets(hovered_widget_, context);
    hovered_widget_ = 0;
  }
}

Response Menu::PerformKeyPress (AbstractWindow* context)
{
  if (context->GetKeyInput() == Key_Escape) {
    RequestRedraw();
    delete this;
    return Finish;
  }

  return Ignore;
}

Response Menu::PerformContextMenuPress (AbstractWindow* context)
{
  return Ignore;
}

Response Menu::PerformContextMenuRelease (AbstractWindow* context)
{
  return Ignore;
}

Response Menu::PerformMouseHover (AbstractWindow* context)
{
  if (pressed_) return Finish;

  if (Contain(context->GetGlobalCursorPosition())) {

    cursor_range_ = InsideRectangle;

    if (!hover_) {
      PerformHoverIn(context);
    }

    AbstractWidget* new_hovered_widget = DispatchMouseHover(hovered_widget_,
                                                            context);

    if (new_hovered_widget != hovered_widget_) {

      if (hovered_widget_) {
        hovered_widget_->destroyed().disconnect1(
            this, &Menu::OnHoverWidgetDestroyed);
      }

      hovered_widget_ = new_hovered_widget;
      if (hovered_widget_) {
        hovered_widget_->destroyed().connect(this,
                          &Menu::OnHoverWidgetDestroyed);
      }

    }

  } else {
    cursor_range_ = OutsideRectangle;
    if (hover_) {
      PerformHoverOut(context);
    }
  }

  return Finish;
}

void Menu::InitializeMenu ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->menu_back().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->menu_back().shadetop,
                            AbstractWindow::theme()->menu_back().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  glGenVertexArrays(2, vao_);
  vbo_.generate();

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

void Menu::OnFocusedWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(focused_widget_ == widget);

  //set_widget_focus_status(widget, false);
  DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
  widget->destroyed().disconnect1(this, &Menu::OnFocusedWidgetDestroyed);

  focused_widget_ = nullptr;
}

void Menu::OnHoverWidgetDestroyed (AbstractWidget* widget)
{
  DBG_ASSERT(hovered_widget_ == widget);

  DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
  widget->destroyed().disconnect1(this, &Menu::OnHoverWidgetDestroyed);

  hovered_widget_ = nullptr;
}

void Menu::SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context)
{
  if (focused_widget_ == widget) return;

  if (focused_widget_) {
    dispatch_focus_off(focused_widget_, context);
    focused_widget_->destroyed().disconnect1(this,
                                               &Menu::OnFocusedWidgetDestroyed);
  }

  focused_widget_ = widget;
  if (focused_widget_) {
    dispatch_focus_on(focused_widget_, context);
    focused_widget_->destroyed().connect(this,
                      &Menu::OnFocusedWidgetDestroyed);
  }
}

} // namespace BlendInt
