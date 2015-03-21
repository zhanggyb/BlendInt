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

#include <gui/brightness-slider.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  BrightnessSlider::BrightnessSlider (Orientation orientation)
  : AbstractSlider<int>(orientation)
  {
    set_round_type(RoundAll);
    set_round_radius(6);
    set_size(90, 14);

    InitializeBrightnessSlider();
  }

  BrightnessSlider::~BrightnessSlider ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  bool BrightnessSlider::IsExpandX () const
  {
    if (orientation() == Horizontal) {
      return true;
    } else {
      return false;
    }
  }

  bool BrightnessSlider::IsExpandY () const
  {
    if (orientation() == Vertical) {
      return true;
    } else {
      return false;
    }
  }

  Size BrightnessSlider::GetPreferredSize () const
  {
    // Same size in ColorWheel
    if (orientation() == Horizontal) {
      return Size(160, 14);
    } else {
      return Size(14, 160);
    }
  }

	Response BrightnessSlider::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.f,
        0.f, 0.f, 1.f);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED), 1);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        0.f);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->regular().outline.data());
    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 GetOutlineVertices(round_type()) * 2 + 2);

		glm::mat4 icon_mvp;

//		icon_mvp = glm::translate(mvp,
//				glm::vec3(size().width() / 2 - m_dot.size().width() / 2,
//						12, 0.0));

    context->icons()->dot()->Draw(size().width() / 2, 12.f);

		return Finish;
	}

	void BrightnessSlider::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			GenerateRoundedVertices(Vertical, 255, 0, &inner_verts, &outer_verts);

	    vbo_.bind(0);
	    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
	    vbo_.bind(1);
	    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
	    vbo_.reset();

			RequestRedraw();
		}

		ReportSizeUpdate(request);
	}

	void BrightnessSlider::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(Vertical, 255, 0, &inner_verts, &outer_verts);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

		RequestRedraw();
	}

	void BrightnessSlider::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(Vertical, 255, 0, &inner_verts, &outer_verts);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void BrightnessSlider::InitializeBrightnessSlider()
	{
		glGenVertexArrays(2, vao_);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(Vertical, 255, 0, &inner_verts, &outer_verts);

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

		vbo_.reset();
		glBindVertexArray(0);
	}

}
