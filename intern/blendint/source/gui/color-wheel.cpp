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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <opengl/opengl.hpp>

#include <gui/color-wheel.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	ColorWheel::ColorWheel()
	: AbstractWidget()
	{
		set_size(160, 160);

		InitializeColorWheel();
	}
	
	ColorWheel::~ColorWheel ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool ColorWheel::Contain (const Point& point) const
	{
		float radius = std::min(size().width(), size().height()) / 2.f;
		radius += 1.f;

		float x = size().width() / 2.f;
		float y = size().height() / 2.f;

		float dist = sqrt (pow(point.x() - x, 2) + pow(point.y() - y, 2));

		if(dist <= radius) {
			return true;
		} else {
			return false;
		}
	}

	bool ColorWheel::IsExpandX () const
	{
		return true;
	}

	bool ColorWheel::IsExpandY () const
	{
		return true;
	}

	Size ColorWheel::GetPreferredSize () const
	{
		// TODO: check theme.pixel
		return Size(160, 160);
	}

	Response ColorWheel::Draw (AbstractWindow* context)
	{
		RefPtr<GLSLProgram> program =
						AbstractWindow::shaders()->widget_triangle_program();
		program->use();

		glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
		        (float) (0.f + size().width() / 2.f),
		        (float) (0.f + size().height() / 2.f));
		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 72 + 2);

		glVertexAttrib4fv(AttributeColor, AbstractWindow::theme()->regular().outline.data());
		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 1);
		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 72 * 2 + 2);

		//glm::mat4 icon_mvp;
		//icon_mvp = glm::translate(mvp, glm::vec3(8.f, 12.f, 0.f));
		context->icons()->dot()->Draw(0.f + size().width() / 2.f + 8.f, 0.f + 12.f);

		return Finish;
	}
	
	void ColorWheel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			int radius = std::min(request.size()->width(), request.size()->height()) / 2;

			inner_->bind();

			GLfloat* ptr = (GLfloat*) inner_->map(GL_READ_WRITE);

			double rad = 0.0;
			float x1 = 0.f;
			float y1 = 0.f;

			ptr = ptr + 6;
			int i = 1;
			for (int angle = -30; angle < 330; angle = angle + 5) {
				rad = angle * M_PI / 180.0;

				x1 = (radius - 1) * cos(rad);
				y1 = (radius - 1) * sin(rad);

				*(ptr) = x1;
				*(ptr + 1) = y1;
				ptr += 6;
				i++;
			}

			rad = 330 * M_PI / 180.0;
			x1 = (radius - 1) * cos(rad);
			y1 = (radius - 1) * sin(rad);

			*(ptr) = x1;
			*(ptr + 1) = y1;

			inner_->unmap();
			inner_->reset();

			outer_->bind();

			ptr = (GLfloat*) outer_->map(GL_READ_WRITE);
			float x2 = 0.f;
			float y2 = 0.f;

			i = 0;
			for (int angle = -30; angle < 330; angle = angle + 5) {
				rad = angle * M_PI / 180.0;

				x1 = (radius - 1) * cos(rad);
				y1 = (radius - 1) * sin(rad);
				x2 = radius * cos(rad);
				y2 = radius * sin(rad);

				*(ptr + 0) = x1;
				*(ptr + 1) = y1;
				*(ptr + 2) = x2;
				*(ptr + 3) = y2;
				ptr += 4;
				i++;
			}

			rad = 330 * M_PI / 180.0;
			x1 = (radius - 1) * cos(rad);
			y1 = (radius - 1) * sin(rad);
			x2 = radius * cos(rad);
			y2 = radius * sin(rad);

			*(ptr + 0) = x1;
			*(ptr + 1) = y1;
			*(ptr + 2) = x2;
			*(ptr + 3) = y2;

			outer_->unmap();
			outer_->reset();

			set_size(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ColorWheel::GenerateWheelVertices (int radius,
					std::vector<GLfloat>& inner_vertices,
					std::vector<GLfloat>& outer_vertices)
	{
		if(inner_vertices.size() != (72 * 6 + 6 + 6))
			inner_vertices.resize(72 * 6 + 6 + 6);

		if(outer_vertices.size() != (72 * 4 + 4))
			outer_vertices.resize(72 * 4 + 4);

		double rad = 0.0;
		float x1 = 0.f;
		float y1 = 0.f;
		float x2 = 0.f;
		float y2 = 0.f;

		// 0 1 2 3 4 5
		// x y r g b a

		// the center point
		inner_vertices[0] = 0.f;
		inner_vertices[1] = 0.f;
		inner_vertices[2] = 1.f;
		inner_vertices[3] = 1.f;
		inner_vertices[4] = 1.f;
		inner_vertices[5] = 1.f;

		int i = 1;
		int j = 0;
		for(int angle = -30; angle < 330; angle = angle + 5) {
			rad = angle * M_PI / 180.0;

			x1 = (radius - 1) * cos(rad);
			y1 = (radius - 1) * sin(rad);
			x2 = radius * cos(rad);
			y2 = radius * sin(rad);

			inner_vertices[i * 6 + 0] = x1;
			inner_vertices[i * 6 + 1] = y1;

			outer_vertices[j * 4 + 0] = x1;
			outer_vertices[j * 4 + 1] = y1;
			outer_vertices[j * 4 + 2] = x2;
			outer_vertices[j * 4 + 3] = y2;

			if(angle == -30) {
				inner_vertices[i * 6 + 2] = 1.f;
				inner_vertices[i * 6 + 3] = 0.f;
				inner_vertices[i * 6 + 4] = 1.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle < 30) {
				inner_vertices[i * 6 + 2] = (30 - angle) / 60.f;
				inner_vertices[i * 6 + 3] = 0.f;
				inner_vertices[i * 6 + 4] = 1.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle == 30) {
				inner_vertices[i * 6 + 2] = 0.f;
				inner_vertices[i * 6 + 3] = 0.f;
				inner_vertices[i * 6 + 4] = 1.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle < 90) {
				inner_vertices[i * 6 + 2] = 0.f;
				inner_vertices[i * 6 + 3] = 1.f - (90 - angle) / 60.f;
				inner_vertices[i * 6 + 4] = 1.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle == 90) {
				inner_vertices[i * 6 + 2] = 0.f;
				inner_vertices[i * 6 + 3] = 1.f;
				inner_vertices[i * 6 + 4] = 1.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle < 150) {
				inner_vertices[i * 6 + 2] = 0.f;
				inner_vertices[i * 6 + 3] = 1.f;
				inner_vertices[i * 6 + 4] = (150 - angle) / 60.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle == 150) {
				inner_vertices[i * 6 + 2] = 0.f;
				inner_vertices[i * 6 + 3] = 1.f;
				inner_vertices[i * 6 + 4] = 0.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle < 210) {
				inner_vertices[i * 6 + 2] = 1.f - (210 - angle) / 60.f;
				inner_vertices[i * 6 + 3] = 1.f;
				inner_vertices[i * 6 + 4] = 0.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle == 210) {
				inner_vertices[i * 6 + 2] = 1.f;
				inner_vertices[i * 6 + 3] = 1.f;
				inner_vertices[i * 6 + 4] = 0.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle < 270) {
				inner_vertices[i * 6 + 2] = 1.f;
				inner_vertices[i * 6 + 3] = (270 - angle) / 60.f;
				inner_vertices[i * 6 + 4] = 0.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else if (angle == 270) {
				inner_vertices[i * 6 + 2] = 1.f;
				inner_vertices[i * 6 + 3] = 0.f;
				inner_vertices[i * 6 + 4] = 0.f;
				inner_vertices[i * 6 + 5] = 1.f;
			} else {
				inner_vertices[i * 6 + 2] = 1.f;
				inner_vertices[i * 6 + 3] = 0.f;
				inner_vertices[i * 6 + 4] = 1.f - (330 - angle) / 60.f;
				inner_vertices[i * 6 + 5] = 1.f;
			}
			i++; j++;
		}

		rad = 330 * M_PI / 180.0;
		x1 = (radius - 1) * cos(rad);
		y1 = (radius - 1) * sin(rad);
		x2 = radius * cos(rad);
		y2 = radius * sin(rad);

		inner_vertices[i * 6 + 0] = x1;
		inner_vertices[i * 6 + 1] = y1;

		outer_vertices[j * 4 + 0] = x1;
		outer_vertices[j * 4 + 1] = y1;
		outer_vertices[j * 4 + 2] = x2;
		outer_vertices[j * 4 + 3] = y2;

		inner_vertices[i * 6 + 2] = 1.f;
		inner_vertices[i * 6 + 3] = 0.f;
		inner_vertices[i * 6 + 4] = 1.f;
		inner_vertices[i * 6 + 5] = 1.f;
	}

	void ColorWheel::InitializeColorWheel ()
	{
		std::vector<GLfloat> inner_vertices;
		std::vector<GLfloat> outer_vertices;

		GenerateWheelVertices(80, inner_vertices, outer_vertices);

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();

		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_vertices.size(), &inner_vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(AttributeCoord);
		glEnableVertexAttribArray(AttributeColor);

		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(AttributeColor, 4, GL_FLOAT, GL_FALSE,	sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(vaos_[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_vertices.size(), &outer_vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
