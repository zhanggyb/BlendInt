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

#include <opengl/opengl.hpp>

#include <gui/close-button.hpp>

#include <gui/abstract-window.hpp>

namespace BlendInt {

  CloseButton::CloseButton ()
  : AbstractButton()
  {
    set_round_type(RoundAll);
    int h = 16 * AbstractWindow::theme()->pixel();
    set_size(h, h);
    set_round_radius(h / 2.f);

    InitializeCloseButtonOnce();
  }

  CloseButton::~CloseButton ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  Size CloseButton::GetPreferredSize () const
  {
    int h = pixel_size(16);
    return Size(h, h);
  }

  void CloseButton::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      set_size(*request.size());

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

      buffer_.bind(0);
      buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                           &inner_verts[0]);
      buffer_.bind(1);
      buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
                           &outer_verts[0]);
      buffer_.reset();

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void CloseButton::PerformRoundTypeUpdate (int round_type)
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

    buffer_.bind(0);
    buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    buffer_.bind(1);
    buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    buffer_.reset();

    RequestRedraw();
  }

  void CloseButton::PerformRoundRadiusUpdate (float radius)
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

    buffer_.bind(0);
    buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    buffer_.bind(1);
    buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    buffer_.reset();

    RequestRedraw();
  }

  Response CloseButton::Draw (AbstractWindow* context)
  {
    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);

    if (is_down()) {
      glUniform4fv(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
          AbstractWindow::theme()->regular().inner_sel.data());
    } else {
      glUniform4fv(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
          AbstractWindow::theme()->regular().inner.data());
    }

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
        0.f, 0.f);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->regular().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 GetOutlineVertices(round_type()) * 2 + 2);

    if (is_down()) {
      glUniform4f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
          1.0f, 1.0f, 1.0f, 0.16f);
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
          0.f, -1.f);
      glDrawArrays(GL_TRIANGLE_STRIP, 0,
                   GetHalfOutlineVertices(round_type()) * 2);
    }

    int x = size().width() / 2;
    int y = size().height() / 2;

    AbstractWindow::icons()->cross()->Draw(x, y, Color(0x0F0F0FFF).data());

    return Finish;
  }

  void CloseButton::InitializeCloseButtonOnce ()
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
    buffer_.generate();

    glBindVertexArray(vao_[0]);

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

}
