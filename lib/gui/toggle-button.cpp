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

#include <blendint/gui/toggle-button.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

ToggleButton::ToggleButton ()
    : AbstractButton()
{
  set_round_type(RoundAll);
  set_checkable(true);

  Font font;	// default font
  int w = 80;
  int h = font.height();

  set_size(w + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));

  InitializeToggleButtonOnce();
}

ToggleButton::ToggleButton (const String& text)
    : AbstractButton(text)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->text()->size().width();
  int h = this->text()->font().height();
  if (w < 80) w = 80;

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeToggleButtonOnce();
}

ToggleButton::ToggleButton (const RefPtr<AbstractIcon>& icon)
    : AbstractButton(icon)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeToggleButtonOnce();
}

ToggleButton::ToggleButton (const RefPtr<AbstractIcon>& icon,
                            const String& text)
    : AbstractButton(icon, text)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += kIconTextSpace;

  w += this->text()->size().width();
  h = std::max(h, this->text()->font().height());

  if (w < 80) w = 80;
  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeToggleButtonOnce();
}

ToggleButton::~ToggleButton ()
{
  glDeleteVertexArrays(2, vao_);
}

bool ToggleButton::IsExpandX () const
{
  return true;
}

Size ToggleButton::GetPreferredSize () const
{
  Size s = AbstractButton::GetPreferredSize();

  if (text()) {
    if (s.width() < 80) {
      s.set_width(80);
    }
  }

  return s;
}

void ToggleButton::PerformSizeUpdate (const AbstractView* source,
                                      const AbstractView* target,
                                      int width,
                                      int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->toggle().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->toggle().shadetop,
                              AbstractWindow::theme()->toggle().shadedown,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void ToggleButton::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->toggle().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->toggle().shadetop,
                            AbstractWindow::theme()->toggle().shadedown,
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

void ToggleButton::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->toggle().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->toggle().shadetop,
                            AbstractWindow::theme()->toggle().shadedown,
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

void ToggleButton::PerformHoverIn (AbstractWindow* context)
{
  if (is_pressed()) {
    set_down(true);
    set_checked(!is_checked());
    RequestRedraw();
  }
}

void ToggleButton::PerformHoverOut (AbstractWindow* context)
{
  if (is_pressed()) {
    set_down(false);
    set_checked(!is_checked());
    RequestRedraw();
  }
}

Response ToggleButton::Draw (AbstractWindow* context)
{
  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              context->theme()->toggle().shaded);

  if (is_checked()) {
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        AbstractWindow::theme()->toggle().inner_sel.data());
  } else {
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        AbstractWindow::theme()->toggle().inner.data());
  }

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  AbstractWindow::shaders()->widget_outer_program()->use();

  glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, AbstractWindow::theme()->toggle().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  if (emboss()) {
    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
        1.0f, 1.0f, 0.16f);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        0.f - 1.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, emboss_vertex_count(round_type()) * 2);
  }

  DrawIconText();

  return Finish;
}

void ToggleButton::InitializeToggleButtonOnce ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->toggle().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->toggle().shadetop,
                            AbstractWindow::theme()->toggle().shadedown,
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

}
