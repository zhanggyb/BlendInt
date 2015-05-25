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

#include <blendint/gui/menu-button.hpp>

#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

MenuButton::MenuButton (const String& text)
    : AbstractButton(text),
      vao_(0),
      hover_(false)
{
  set_round_type(RoundAll);

  int w = this->text()->size().width();
  int h = this->text()->font().height();

  w += kPadding.hsum();
  h += kPadding.vsum();

  set_size(w, h);

  InitializeMenuButton();
}

MenuButton::~MenuButton ()
{
  glDeleteVertexArrays(1, &vao_);
}

void MenuButton::PerformSizeUpdate (const AbstractView* source,
                                    const AbstractView* target,
                                    int width,
                                    int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, round_type(), round_radius(),
                     &inner_verts, 0);

    vbo_.bind();
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                      &inner_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void MenuButton::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type, round_radius(), &inner_verts, 0);

  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void MenuButton::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                   0);

  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

Response MenuButton::Draw (AbstractWindow* context)
{
  if (hover_) {

    shaders()->widget_inner_program()->use();

    glUniform1i(
        shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        shaders()->location(Shaders::WIDGET_INNER_SHADED),
        theme()->menu_item().shaded);
    glUniform4fv(
        shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        theme()->menu_item().inner_sel.data());

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  }

  DrawIconText();

  return Finish;
}

void MenuButton::PerformHoverIn (AbstractWindow* context)
{
  if (!hover_) {
    hover_ = true;
    RequestRedraw();
  }

  return AbstractButton::PerformHoverIn(context);
}

void MenuButton::PerformHoverOut (AbstractWindow* context)
{
  if (hover_) {
    hover_ = false;
    RequestRedraw();
  }

  return AbstractButton::PerformHoverOut(context);
}

void MenuButton::InitializeMenuButton ()
{
  glGenVertexArrays(1, &vao_);

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                   0);

  glBindVertexArray(vao_);
  vbo_.generate();
  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

} /* namespace BlendInt */
