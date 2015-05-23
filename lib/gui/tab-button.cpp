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

#include <cmath>

#include <blendint/gui/tab-button.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

TabButton::TabButton ()
    : AbstractButton()
{
  set_round_type(RoundAll);
  set_checkable(true);

  Font font;  // default font
  int h = font.height();
  int w = h;

  set_size(w + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));

  InitializeTabButtonOnce();
}

TabButton::TabButton (const String& text)
    : AbstractButton(text)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->text()->size().width();
  int h = this->text()->font().height();

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeTabButtonOnce();
}

TabButton::TabButton (const RefPtr<AbstractIcon>& icon)
    : AbstractButton(icon)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeTabButtonOnce();
}

TabButton::TabButton (const RefPtr<AbstractIcon>& icon, const String& text)
    : AbstractButton(icon, text)
{
  set_round_type(RoundAll);
  set_checkable(true);

  int w = this->icon()->size().width();
  int h = this->icon()->size().height();

  w += kIconTextSpace;

  w += this->text()->size().width();
  h = std::max(h, this->text()->font().height());

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  InitializeTabButtonOnce();
}

TabButton::~TabButton ()
{
  glDeleteVertexArrays(2, vao_);
}

void TabButton::PerformSizeUpdate (const AbstractView* source,
                                   const AbstractView* target,
                                   int width,
                                   int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (theme()->tab().shaded) {
      GenerateRoundedVertices(Vertical, theme()->tab().shadetop,
                              theme()->tab().shadedown,
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

void TabButton::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (theme()->tab().shaded) {
    GenerateRoundedVertices(Vertical, theme()->tab().shadetop,
                            theme()->tab().shadedown,
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

void TabButton::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (theme()->tab().shaded) {
    GenerateRoundedVertices(Vertical, theme()->tab().shadetop,
                            theme()->tab().shadedown,
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

Response TabButton::Draw (AbstractWindow* context)
{
  shaders()->widget_inner_program()->use();

  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_SHADED),
              theme()->tab().shaded);

  if (is_checked()) {
    glUniform4fv(
        shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        theme()->tab().inner_sel.data());
  } else {
    glUniform4fv(
        shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        theme()->tab().inner.data());
  }

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  shaders()->widget_outer_program()->use();

  glUniform2f(shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);
  glUniform4fv(shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, theme()->tab().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  if (emboss()) {
    glUniform4f(
        shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
        1.0f, 1.0f, 0.16f);
    glUniform2f(
        shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        0.f - 1.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, emboss_vertex_count(round_type()) * 2);
  }

  if (is_down()) {
    DrawIconText(theme()->tab().text_sel.data(), 0);
  } else {
    DrawIconText(theme()->tab().text.data(), 0);
  }

  return Finish;
}

void TabButton::InitializeTabButtonOnce ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (theme()->tab().shaded) {
    GenerateRoundedVertices(Vertical, theme()->tab().shadetop,
                            theme()->tab().shadedown,
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
