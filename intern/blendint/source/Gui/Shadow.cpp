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

#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Gui/Shadow.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FrameShadow::FrameShadow(const Size& size, int round_type, float round_radius)
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(size);

		if(round_radius < 1.f)
			round_radius = 1.f;

		set_radius(round_radius);

		round_type &= 0x0F;
		round_type |= (RoundTopLeft | RoundTopRight);

		set_round_type(round_type);

		InitializeShadowMap();
	}

	FrameShadow::~FrameShadow()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void FrameShadow::Draw(float x, float y, short gamma) const
	{
		Shaders::instance->frame_shadow_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_SHADOW_POSITION), x, y);

		glBindVertexArray(vao_);

		int count = GetOutlineVertexCount(round_type());
		for(int i = 0; i < 12; i++) {
			glDrawElements(GL_TRIANGLE_STRIP, count * 2, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * count * 2 * i));
		}


		glBindVertexArray(0);

		GLSLProgram::reset();
	}

	void FrameShadow::PerformSizeUpdate(const Size& size)
	{
		set_size(size);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		//buffer_.set_sub_data(0, sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();
	}

	void FrameShadow::PerformRoundTypeUpdate(int type)
	{
		type &= 0x0F;
		type |= (RoundTopLeft | RoundTopRight);

		if(type == round_type()) return;

		set_round_type(type);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();
	}

	void FrameShadow::PerformRoundRadiusUpdate(float radius)
	{
		if(radius < 1.f) radius = 1.f;

		if(radius == this->radius()) return;

		set_radius(radius);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();

	}

	void FrameShadow::InitializeShadowMap()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.generate();
		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_COORD));

		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		element_buffer_.generate();
		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);

		glBindVertexArray(0);

		vertex_buffer_.reset();
		element_buffer_.reset();
	}

	void FrameShadow::GenerateShadowVertices(std::vector<GLfloat>& vertices, std::vector<GLuint>& elements)
	{
		int width = 12;

		float rad = radius() * Theme::instance->pixel();

		float minx = 0.0f;
		float miny = 0.0f;
		float maxx = size().width();
		float maxy = size().height();

		maxy -= 2 * rad;

		float vec[WIDGET_CURVE_RESOLU][2];

		width *= Theme::instance->pixel();

		int outline_vertex_count = GetOutlineVertexCount(round_type());
		unsigned int verts_num = (width + 1) * outline_vertex_count * 3;	// 3 float for one vertex: 0, 1: coord, 2: shade

		if(vertices.size() != verts_num) {
			vertices.resize(verts_num);
		}

		float shade = 1.f;
		int count = 0;
		for(int i = 0; i <= width; i++) {

			for(int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
				vec[j][0] = rad * cornervec[j][0];
				vec[j][1] = rad * cornervec[j][1];
			}

			//shade = 1.0 - std::sqrt(i * (1.0 / width));
			shade = 1.0 - std::pow(i * (1.0 / width), 1.0 / 3);

			DBG_PRINT_MSG("shade: %f", shade);

			// for shadow, start from left-top

			// corner left-top
			if (round_type() & RoundTopLeft) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
					vertices[count + 0] = minx + rad - vec[j][0];
					vertices[count + 1] = maxy - vec[j][1];
					vertices[count + 2] = shade;
					count += 3;
				}
			} else {
				vertices[count + 0] = minx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = shade;
				count += 3;
			}

			// corner left-bottom
			if (round_type() & RoundBottomLeft) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
					vertices[count + 0] = minx + vec[j][1];
					vertices[count + 1] = miny + rad - vec[j][0];
					vertices[count + 2] = shade;
					count += 3;
				}
			} else {
				vertices[count + 0] = minx;
				vertices[count + 1] = miny;
				vertices[count + 2] = shade;
				count += 3;
			}

			// corner right-bottom
			if (round_type() & RoundBottomRight) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
					vertices[count + 0] = maxx - rad + vec[j][0];
					vertices[count + 1] = miny + vec[j][1];
					vertices[count + 2] = shade;
					count += 3;
				}
			} else {
				vertices[count + 0] = maxx;
				vertices[count + 1] = miny;
				vertices[count + 2] = shade;
				count += 3;
			}

			// corner right-top
			if (round_type() & RoundTopRight) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
					vertices[count + 0] = maxx - vec[j][1];
					vertices[count + 1] = maxy - rad + vec[j][0];
					vertices[count + 2] = shade;
					count += 3;
				}
			} else {
				vertices[count + 0] = maxx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = shade;
				count += 3;
			}

			rad += 1.f;
			minx -= 1.f;
			miny -= 1.f;
			maxx += 1.f;
			maxy += 1.f;
		}

		assert(count == (int)verts_num);

		unsigned int elements_num = outline_vertex_count * 2 * width;

		if(elements.size() != elements_num) {
			elements.resize(elements_num);
		}

		count = 0;
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < (int)outline_vertex_count; j++) {
				elements[count + 0] = i * outline_vertex_count + j;
				elements[count + 1] = (i + 1) * outline_vertex_count + j;
				count += 2;
			}
		}

		assert(count == (int)elements_num);
	}

	int FrameShadow::GetOutlineVertexCount (int round_type)
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return (4 - count) + count * WIDGET_CURVE_RESOLU;
	}

}
