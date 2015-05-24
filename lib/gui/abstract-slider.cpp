/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/abstract-slider.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

  SlideIcon::SlideIcon ()
      : AbstractRoundForm()
  {
    set_size(14, 14);
    set_round_type(RoundAll);
    set_radius(7.0);

    InitializeSliderIcon();
  }

  SlideIcon::~SlideIcon ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void SlideIcon::PerformSizeUpdate (int width, int height)
  {
    set_size(width, height);

    Orientation shadedir = width < height ? Horizontal : Vertical;
    short shadetop = theme()->scroll().shadetop;
    short shadedown = theme()->scroll().shadedown;

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(shadedir, shadetop, shadedown, &inner_verts,
                            &outer_verts);

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();
  }

  void SlideIcon::PerformRoundTypeUpdate (int type)
  {
    set_round_type(type);

    Orientation shadedir =
        size().width() < size().height() ? Horizontal : Vertical;
    short shadetop = theme()->scroll().shadetop;
    short shadedown = theme()->scroll().shadedown;

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(shadedir, shadetop, shadedown, &inner_verts,
                            &outer_verts);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();
  }

  void SlideIcon::PerformRoundRadiusUpdate (float radius)
  {
    set_radius(radius);

    Orientation shadedir =
        size().width() < size().height() ? Horizontal : Vertical;
    short shadetop = theme()->scroll().shadetop;
    short shadedown = theme()->scroll().shadedown;

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(shadedir, shadetop, shadedown, &inner_verts,
                            &outer_verts);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();
  }

  void SlideIcon::Draw (int x,
                        int y,
                        const float* color_ptr,
                        short gamma,
                        float rotate,
                        float scale_x,
                        float scale_y) const
  {
    shaders()->widget_simple_triangle_program()->use();

    glUniform2f(shaders()->location(Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION),
                pixel_size(x),
                pixel_size(y));
    glUniform4fv(shaders()->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR),
                 1, theme()->scroll().item.data());
    glUniform1i(shaders()->location(Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA),
                gamma);

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertexCount(round_type()) + 2);

    shaders()->widget_outer_program()->use();

    glUniform2f(shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
                pixel_size(x),
                pixel_size(y));
    glUniform4fv(
        shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        theme()->scroll().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 GetOutlineVertexCount(round_type()) * 2 + 2);
  }

  void SlideIcon::InitializeSliderIcon ()
  {
    glGenVertexArrays(2, vao_);

    Orientation shadedir =
        size().width() < size().height() ? Horizontal : Vertical;
    short shadetop = theme()->scroll().shadetop;
    short shadedown = theme()->scroll().shadedown;

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(shadedir, shadetop, shadedown, &inner_verts,
                            &outer_verts);

    vbo_.generate();

    glBindVertexArray(vao_[0]);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

    glEnableVertexAttribArray(
        shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_COORD));
    glVertexAttribPointer(
        shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_COORD),
        3,
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

}  // namespace BlendInt
