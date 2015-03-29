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
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <opengl/gl-framebuffer.hpp>

#include <gui/flow-layout.hpp>
#include <gui/abstract-window.hpp>
#include <gui/panel.hpp>

namespace BlendInt {

Panel::Panel (AbstractLayout* layout)
    : AbstractRoundWidget(), layout_(0)
{
  if (layout == nullptr) {
    layout_ = Manage(new FlowLayout);
  } else {
    layout_ = layout;
  }

  PushBackSubView(layout_);

  set_size(layout_->size());

  view_buffer_.reset(new ViewBuffer(size().width(), size().height()));

  InitializePanelOnce();
}

Panel::~Panel ()
{
  glDeleteVertexArrays(2, vao_);
}

void Panel::AddWidget (AbstractWidget* widget)
{
  if (layout_) {
    layout_->AddWidget(widget);
  } else {
    PushBackSubView(widget);
  }

  RequestRedraw();
}

void Panel::InsertWidget (int index, AbstractWidget* widget)
{
  if (layout_) {
    layout_->InsertWidget(index, widget);
  } else {
    InsertSubView(index, widget);
  }

  RequestRedraw();
}

bool Panel::IsExpandX () const
{
  if (layout_) {
    return layout_->IsExpandX();
  }

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandX()) return true;
  }

  return false;
}

bool Panel::IsExpandY () const
{
  if (layout_) {
    return layout_->IsExpandY();
  }

  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandY()) return true;
  }

  return false;
}

Size Panel::GetPreferredSize () const
{
  Size prefer_size;

  if (subview_count() == 0) {
    prefer_size.reset(400, 300);
  } else {

    if (layout_) {
      DBG_ASSERT(subview_count() == 1);
      prefer_size = layout_->GetPreferredSize();
    } else {

      int minx = 0;
      int miny = 0;
      int maxx = 0;
      int maxy = 0;

      for (AbstractView* p = first(); p; p = next(p)) {
        minx = std::min(minx, p->position().x());
        miny = std::min(miny, p->position().y());
        maxx = std::max(maxx, p->position().x() + p->size().width());
        maxy = std::max(maxy, p->position().y() + p->size().height());
      }

      prefer_size.reset(maxx - minx, maxy - miny);
    }
  }

  return prefer_size;
}

void Panel::PerformSizeUpdate (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->regular().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->regular().shadetop,
                              AbstractWindow::theme()->regular().shadedown,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

    if (view_buffer_) view_buffer_->Resize(size());

    if (layout_) ResizeSubView(layout_, size());

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void Panel::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->regular().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->regular().shadetop,
                            AbstractWindow::theme()->regular().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void Panel::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->regular().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->regular().shadetop,
                            AbstractWindow::theme()->regular().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

Response Panel::Draw (AbstractWindow* context)
{
  Response retval = Finish;

  if (refresh() && view_buffer_) {
    //DBG_PRINT_MSG("%s", "refresh once");
    RenderSubWidgetsToTexture(this, context, view_buffer_->texture());
  }

  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              0);

  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
               1, AbstractWindow::theme()->regular().inner.data());

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  if (view_buffer_) {

    AbstractWindow::shaders()->widget_image_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION),
        0.f, 0.f);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    view_buffer_->Draw(0, 0);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    retval = Finish;

  } else {

    retval = Ignore;

  }

  AbstractWindow::shaders()->widget_outer_program()->use();

  glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, AbstractWindow::theme()->regular().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  if (emboss()) {
    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
        1.0f, 1.0f, 0.16f);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        -1.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, emboss_vertex_count(round_type()) * 2);
  }

  return retval;
}

AbstractView* Panel::RemoveSubView (AbstractView* view)
{
  DBG_PRINT_MSG("%s", "Remove layout");

  DBG_ASSERT(view == layout_);
  layout_ = 0;

  return AbstractRoundWidget::RemoveSubView(view);
}

void Panel::InitializePanelOnce ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->regular().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->regular().shadetop,
                            AbstractWindow::theme()->regular().shadedown,
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
  glVertexAttribPointer(AttributeCoord, 3,
  GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2,
  GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

} /* namespace BlendInt */
