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

#include <blendint/gui/push-button.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

PushButton::PushButton ()
    : AbstractButton()
{
  set_round_type(RoundAll);

  Font font;	// default font
  int w = 80;
  int h = font.height();

  set_size(w + kPadding.hsum(), h + kPadding.vsum());

  InitializeButtonOnce();
}

PushButton::PushButton (const String& text)
    : AbstractButton(text)
{
  set_round_type(RoundAll);

  int w = this->text()->size().width();
  int h = this->text()->font().height();
  if (w < 80) w = 80;

  w += kPadding.hsum();
  h += kPadding.vsum();

  set_size(w, h);

  InitializeButtonOnce();
}

PushButton::PushButton (const RefPtr<AbstractIcon>& icon)
    : AbstractButton(icon)
{
  set_round_type(RoundAll);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += kPadding.hsum();
  h += kPadding.vsum();

  set_size(w, h);

  InitializeButtonOnce();
}

PushButton::PushButton (const RefPtr<AbstractIcon>& icon, const String& text)
    : AbstractButton(icon, text)
{
  set_round_type(RoundAll);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += kIconTextSpace;

  w += this->text()->size().width();
  h = std::max(h, this->text()->font().height());

  if (w < 80) w = 80;
  w += kPadding.hsum();
  h += kPadding.vsum();

  set_size(w, h);

  InitializeButtonOnce();
}

PushButton::~PushButton ()
{
  glDeleteVertexArrays(2, vao_);
}

Size PushButton::GetPreferredSize () const
{
  Size s = AbstractButton::GetPreferredSize();

  if (text()) {
    if (s.width() < 80) {
      s.set_width(80);
    }
  }

  return s;
}

void PushButton::SetText (const String& text)
{
  set_text(text);
  RequestRedraw();
}

void PushButton::SetIcon (const RefPtr<AbstractIcon>& icon)
{
  set_icon(icon);
  RequestRedraw();
}

void PushButton::PerformSizeUpdate (const AbstractView* source,
                                    const AbstractView* target,
                                    int width,
                                    int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(Vertical, AbstractWindow::theme()->push_button(),
                            &inner_verts, &outer_verts);

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

void PushButton::PerformRoundTypeUpdate (int type)
{
  set_round_type(type);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateRoundedVertices(Vertical, AbstractWindow::theme()->push_button(),
                          &inner_verts, &outer_verts);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void PushButton::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateRoundedVertices(Vertical, AbstractWindow::theme()->push_button(),
                          &inner_verts, &outer_verts);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void PushButton::PerformHoverIn (AbstractWindow* context)
{
  if (is_pressed()) {
    set_down(true);
    RequestRedraw();
  }
}

void PushButton::PerformHoverOut (AbstractWindow* context)
{
  if (is_pressed()) {
    set_down(false);
    RequestRedraw();
  }
}

Response PushButton::Draw (AbstractWindow* context)
{
  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              context->theme()->push_button().shaded);
  if (is_down()) {
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        AbstractWindow::theme()->push_button().inner_sel.data());
  } else {
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        AbstractWindow::theme()->push_button().inner.data());
  }

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  AbstractWindow::shaders()->widget_outer_program()->use();

  glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, AbstractWindow::theme()->push_button().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  if (emboss()) {
    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
        1.0f, 1.0f, 0.16f);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        pixel_size(-1.f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, emboss_vertex_count(round_type()) * 2);
  }

  DrawIconText();

  return Finish;
}

void PushButton::InitializeButtonOnce ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateRoundedVertices(Vertical,
                          AbstractWindow::theme()->push_button(),
                          &inner_verts,
                          &outer_verts);

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
  glVertexAttribPointer(AttributeCoord, 2,
  GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

}
