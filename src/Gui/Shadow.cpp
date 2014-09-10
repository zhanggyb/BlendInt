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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include <BlendInt/Gui/Shadow.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Shadow::Shadow()
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(100, 100);
		color_.set_value(0.f, 0.f, 0.f, 0.33f);

		InitializeShadow();
	}

	Shadow::Shadow(const Size& size, int round_type, float radius)
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(size);
		set_round_type(round_type);
		set_radius(radius);
		color_.set_value(0.f, 0.f, 0.f, 0.33f);

		InitializeShadow();
	}

	Shadow::~Shadow()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Shadow::SetColor(const Color& color)
	{
		if(color == color_) return;

		color_ = color;

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();
	}

	void Shadow::Draw (const glm::vec3& pos, short gamma)
	{
		Shaders::instance->triangle_program()->use();

		int count = GetOutlineVertices(round_type());

		glUniform3fv(Shaders::instance->location(Stock::TRIANGLE_POSITION), 1,
		        glm::value_ptr(pos));
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), gamma);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 1);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (count + 1) * 2);
		glBindVertexArray(0);

		GLSLProgram::reset();
	}

	void Shadow::PerformSizeUpdate(const Size& size)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size, round_type(), radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();
	}

	void Shadow::PerformRoundTypeUpdate(int type)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), type, radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();
	}

	void Shadow::PerformRoundRadiusUpdate(float radius)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius, vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();
	}

	void Shadow::InitializeShadow()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius(), vertices);

		buffer_.reset(new GLArrayBuffer);
		buffer_->generate();
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COLOR));

		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		        BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COLOR), 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void Shadow::GenerateShadowVertices(const Size& size, int round_type,
			float radius, std::vector<GLfloat>& vertices)
	{
		const float minx = 0.0f;
		const float miny = 0.0f;
		const float maxx = size.width();
		const float maxy = size.height();

		short x_offset = Theme::instance->shadow_offset_x();
		short y_offset = Theme::instance->shadow_offset_y();

		if (std::abs(x_offset) >= Theme::instance->shadow_width()) {
			x_offset = x_offset > 0 ? (Theme::instance->shadow_width() - 1) : (Theme::instance->shadow_width() + 1);
		}

		if (std::abs(x_offset) >= Theme::instance->shadow_width()) {
			y_offset = y_offset > 0 ? (Theme::instance->shadow_width() - 1) : (Theme::instance->shadow_width() + 1);
		}

		if (2.0f * radius > size.height())
			radius = 0.5f * size.height();

		int edge_vertex_count = GetOutlineVertices(round_type);
		unsigned int max_count = (edge_vertex_count + 1) * 6 * 2;

		if(vertices.size() != max_count) vertices.resize(max_count);

		int count = 0;

		float offset = Theme::instance->shadow_width();
		float radi = radius;
		float rado = radi + Theme::instance->shadow_width();

		// TODO: fine tune shade 1 ~ 2 for better look
		const float shade1 = -5.f / 255.f;
		const float shade2 = 27.5 / 255.f;

		short shadetop_x = x_offset * 5;
		short shadedown_x = -x_offset * 5;
		short shadetop_y = y_offset * 5;
		short shadedown_y = -y_offset * 5;

		const float facx = (maxx != minx) ? 1.0f / (maxx - minx) : 0.0f;
		const float facy = (maxy != miny) ? 1.0f / (maxy - miny) : 0.0f;

		float fx = 0.f;
		float fy = 0.f;

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi - radi * cornervec[j][0];
				vertices[count + 1] = maxy - radi * cornervec[j][1];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 + fx - fy;
				vertices[count + 3] = color_.green() + shade1 + fx - fy;
				vertices[count + 4] = color_.blue() + shade1 + fx - fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = minx - offset + rado - rado * cornervec[j][0] + x_offset;
				vertices[count + 7] = maxy + offset - rado * cornervec[j][1] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = maxy;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 0.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 1.f);

			vertices[count + 2] = color_.red() + shade1 + fx - fy;
			vertices[count + 3] = color_.green() + shade1 + fx - fy;
			vertices[count + 4] = color_.blue() + shade1 + fx - fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = minx - offset + x_offset;
			vertices[count + 7] = maxy + offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundBottomLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi * cornervec[j][1];
				vertices[count + 1] = miny + radi - radi * cornervec[j][0];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 + fx + fy;
				vertices[count + 3] = color_.green() + shade1 + fx + fy;
				vertices[count + 4] = color_.blue() + shade1 + fx + fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = minx - offset + rado * cornervec[j][1] + x_offset;
				vertices[count + 7] = miny - offset + rado - rado * cornervec[j][0] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = miny;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 0.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 0.f);

			vertices[count + 2] = color_.red() + shade1 + fx + fy;
			vertices[count + 3] = color_.green() + shade1 + fx + fy;
			vertices[count + 4] = color_.blue() + shade1 + fx + fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = minx - offset + x_offset;
			vertices[count + 7] = miny - offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundBottomRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi + radi * cornervec[j][0];
				vertices[count + 1] = miny + radi * cornervec[j][1];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 - fx + fy;
				vertices[count + 3] = color_.green() + shade1 - fx + fy;
				vertices[count + 4] = color_.blue() + shade1 - fx + fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = maxx + offset - rado + rado * cornervec[j][0] + x_offset;
				vertices[count + 7] = miny - offset + rado * cornervec[j][1] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = miny;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 1.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 0.f);

			vertices[count + 2] = color_.red() + shade1 - fx + fy;
			vertices[count + 3] = color_.green() + shade1 - fx + fy;
			vertices[count + 4] = color_.blue() + shade1 - fx + fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = maxx + offset + x_offset;
			vertices[count + 7] = miny - offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundTopRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi * cornervec[j][1];
				vertices[count + 1] = maxy - radi + radi * cornervec[j][0];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 - fx - fy;
				vertices[count + 3] = color_.green() + shade1 - fx - fy;
				vertices[count + 4] = color_.blue() + shade1 - fx - fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = maxx + offset - rado * cornervec[j][1] + x_offset;
				vertices[count + 7] = maxy + offset - rado + rado * cornervec[j][0] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = maxy;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 1.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 1.f);

			vertices[count + 2] = color_.red() + shade1 - fx - fy;
			vertices[count + 3] = color_.green() + shade1 - fx - fy;
			vertices[count + 4] = color_.blue() + shade1 - fx - fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = maxx + offset + x_offset;
			vertices[count + 7] = maxy + offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		vertices[count + 0] = vertices[count - edge_vertex_count * 12 + 0];
		vertices[count + 1] = vertices[count - edge_vertex_count * 12 + 1];
		vertices[count + 2] = vertices[count - edge_vertex_count * 12 + 2];
		vertices[count + 3] = vertices[count - edge_vertex_count * 12 + 3];
		vertices[count + 4] = vertices[count - edge_vertex_count * 12 + 4];
		vertices[count + 5] = vertices[count - edge_vertex_count * 12 + 5];

		vertices[count + 6] = vertices[count - edge_vertex_count * 12 + 6];
		vertices[count + 7] = vertices[count - edge_vertex_count * 12 + 7];
		vertices[count + 8] = vertices[count - edge_vertex_count * 12 + 8];
		vertices[count + 9] = vertices[count - edge_vertex_count * 12 + 9];
		vertices[count + 10] = vertices[count - edge_vertex_count * 12 + 10];
		vertices[count + 11] = vertices[count - edge_vertex_count * 12 + 11];

		count += 12;

	}

	inline float Shadow::make_shaded_offset(short shadetop, short shadedown,
			float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

}
