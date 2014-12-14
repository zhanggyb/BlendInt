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

	ShadowMap::ShadowMap(const Size& size, int round_type, float round_radius)
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(size);
		set_round_type(round_type);
		set_radius(round_radius);

		InitializeShadowMap();
	}

	ShadowMap::~ShadowMap()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ShadowMap::Draw(float x, float y, short gamma) const
	{
		Shaders::instance->frame_shadow_program()->use();

		//int count = GetOutlineVertices(round_type());

		glUniform2f(Shaders::instance->location(Stock::FRAME_SHADOW_POSITION), x, y);
		glUniform1f(Shaders::instance->location(Stock::FRAME_SHADOW_FACTOR), 1.f);
		Theme::instance->shadow_texture()->bind();

		glBindVertexArray(vao_);

		int count = 0;
		int corner = round_type();
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int total_vertex_number = (4 - count) * 3 + count * WIDGET_CURVE_RESOLU;

//		glDrawArrays(GL_TRIANGLE_STRIP, 0, total_vertex_number * 2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 24 + 2);

		glBindVertexArray(0);

		Theme::instance->shadow_texture()->reset();

		GLSLProgram::reset();
	}

	void ShadowMap::PerformSizeUpdate(const Size& size)
	{
		set_size(size);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);

		DBG_PRINT_MSG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!size: %ld", vertices.size());

		//GenerateShadowVertices(vertices);
		buffer_.bind();
		//buffer_.set_sub_data(0, sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::PerformRoundTypeUpdate(int type)
	{
		set_round_type(type);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);
		//GenerateShadowVertices(vertices);
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::PerformRoundRadiusUpdate(float radius)
	{
		set_radius(radius);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);
		//GenerateShadowVertices(vertices);
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::InitializeShadowMap()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;

		GenerateShadowVertices(vertices);
		//GenerateShadowVertices(vertices);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_UV));

		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_COORD), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_UV), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	void ShadowMap::GenerateShadowVertices(std::vector<GLfloat>& vertices)
	{
		const float minx = 0.0f;
		const float miny = 0.0f;
		const float maxx = size().width();
		const float maxy = size().height();

		float width = 100.f;	// TODO: move to Theme

		int count = 0;
		int corner = round_type();
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int total_vertex_number = (4 - count) * 3 + count * WIDGET_CURVE_RESOLU;
		DBG_PRINT_MSG("total vertex number: %u", total_vertex_number);

		// DEBUG

		/*
		vertices.resize(6 * 4, 0.f);

		vertices[4 * 0 + 0] = minx;
		vertices[4 * 0 + 1] = maxy;
		vertices[4 * 0 + 2] = 1.f;
		vertices[4 * 0 + 3] = 1.f;

		vertices[4 * 1 + 0] = minx;
		vertices[4 * 1 + 1] = maxy + width;
		vertices[4 * 1 + 2] = 1.f;
		vertices[4 * 1 + 3] = 0.f;

		vertices[4 * 2 + 0] = minx;
		vertices[4 * 2 + 1] = maxy;
		vertices[4 * 2 + 2] = 1.f;
		vertices[4 * 2 + 3] = 1.f;

		vertices[4 * 3 + 0] = minx - width;
		vertices[4 * 3 + 1] = maxy + width;
		vertices[4 * 3 + 2] = 0.f;
		vertices[4 * 3 + 3] = 0.f;

		vertices[4 * 4 + 0] = minx;
		vertices[4 * 4 + 1] = maxy;
		vertices[4 * 4 + 2] = 1.f;
		vertices[4 * 4 + 3] = 1.f;

		vertices[4 * 5 + 0] = minx - width;
		vertices[4 * 5 + 1] = maxy;
		vertices[4 * 5 + 2] = 0.f;
		vertices[4 * 5 + 3] = 1.f;

		// END DEBUG

		return;
		*/

		/*
		if(vertices.size() != total_vertex_number * 2 * 4) {
			vertices.resize(total_vertex_number * 2 * 4, 0.f);
		}
		*/
		vertices.resize(24 * 4 + 2 * 4, 0.f);

		count = 0;
		corner = round_type();

		// corner left-bottom
		if(corner & RoundBottomLeft) {

			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vertices[count * 8 + 0] = minx;
				vertices[count * 8 + 1] = 0.f;
				vertices[count * 8 + 2] = 0.f;
				vertices[count * 8 + 3] = 0.f;

				vertices[count * 8 + 4] = 0.f;
				vertices[count * 8 + 5] = 0.f;
				vertices[count * 8 + 6] = 0.f;
				vertices[count * 8 + 7] = 0.f;

				count += 8;
			}

		} else {

			if(corner & RoundBottomRight) {	// the next corner is round?

				for(int i = 0; i < 3; i++) {

					vertices[count * 8 + 0] = 0.f;
					vertices[count * 8 + 1] = 0.f;
					vertices[count * 8 + 2] = 0.f;
					vertices[count * 8 + 3] = 0.f;

					vertices[count * 8 + 4] = 0.f;
					vertices[count * 8 + 5] = 0.f;
					vertices[count * 8 + 6] = 0.f;
					vertices[count * 8 + 7] = 0.f;

					count += 8;
				}

			} else {

				DBG_PRINT_MSG("Bottom-Left: %s", "next corner is a right-angle");

				vertices[count + 0] = minx;
				vertices[count + 1] = miny;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = minx - width;
				vertices[count + 1] = miny;
				vertices[count + 2] = 0.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = miny;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = minx - width;
				vertices[count + 1] = miny - width;
				vertices[count + 2] = 0.f;
				vertices[count + 3] = 0.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = miny;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = miny - width;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = 0.f;
				count += 4;

				/*
				for(int i = 0; i < 3; i++) {

					vertices[count * 8 + 0] = minx;
					vertices[count * 8 + 1] = miny;
					vertices[count * 8 + 2] = 100 / 512.f;
					vertices[count * 8 + 3] = 100 / 512.f;

					if(i == 0) {
						vertices[count * 8 + 4] = minx - width;
						vertices[count * 8 + 5] = miny;
						vertices[count * 8 + 6] = 0.f;
						vertices[count * 8 + 7] = 100 / 512.f;
					} else if (i == 1) {
						vertices[count * 8 + 4] = minx - width;
						vertices[count * 8 + 5] = miny - width;
						vertices[count * 8 + 6] = 0.f;
						vertices[count * 8 + 7] = 0.f;
					} else {
						vertices[count * 8 + 4] = minx;
						vertices[count * 8 + 5] = miny - width;
						vertices[count * 8 + 6] = 100 / 512.f;
						vertices[count * 8 + 7] = 0.f;
					}

					count += 8;
				}
				*/

			}

		}

		// corner right-bottom
		if(corner & RoundBottomRight) {

			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vertices[count * 8 + 0] = minx;
				vertices[count * 8 + 1] = 0.f;
				vertices[count * 8 + 2] = 0.f;
				vertices[count * 8 + 3] = 0.f;

				vertices[count * 8 + 4] = 0.f;
				vertices[count * 8 + 5] = 0.f;
				vertices[count * 8 + 6] = 0.f;
				vertices[count * 8 + 7] = 0.f;

				count += 8;
			}

		} else {

			if(corner & RoundTopRight) {	// the next corner is round?

				for(int i = 0; i < 3; i++) {

					vertices[count * 8 + 0] = 0.f;
					vertices[count * 8 + 1] = 0.f;
					vertices[count * 8 + 2] = 0.f;
					vertices[count * 8 + 3] = 0.f;

					vertices[count * 8 + 4] = 0.f;
					vertices[count * 8 + 5] = 0.f;
					vertices[count * 8 + 6] = 0.f;
					vertices[count * 8 + 7] = 0.f;

					count += 8;
				}

			} else {

				DBG_PRINT_MSG("Bottom-Right: %s", "next corner is a right-angle");

				vertices[count + 0] = maxx;
				vertices[count + 1] = miny;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = miny - width;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = 0.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = miny;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = maxx + width;
				vertices[count + 1] = miny - width;
				vertices[count + 2] = 1.f;
				vertices[count + 3] = 0.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = miny;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

				vertices[count + 0] = maxx + width;
				vertices[count + 1] = miny;
				vertices[count + 2] = 1.f;
				vertices[count + 3] = 100 / 512.f;
				count += 4;

			}

		}

		// corner right-top
		if(corner & RoundTopRight) {

			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vertices[count * 8 + 0] = minx;
				vertices[count * 8 + 1] = 0.f;
				vertices[count * 8 + 2] = 0.f;
				vertices[count * 8 + 3] = 0.f;

				vertices[count * 8 + 4] = 0.f;
				vertices[count * 8 + 5] = 0.f;
				vertices[count * 8 + 6] = 0.f;
				vertices[count * 8 + 7] = 0.f;

				count += 8;
			}

		} else {

			if(corner & RoundTopLeft) {	// the next corner is round?

				for(int i = 0; i < 3; i++) {

					vertices[count * 8 + 0] = 0.f;
					vertices[count * 8 + 1] = 0.f;
					vertices[count * 8 + 2] = 0.f;
					vertices[count * 8 + 3] = 0.f;

					vertices[count * 8 + 4] = 0.f;
					vertices[count * 8 + 5] = 0.f;
					vertices[count * 8 + 6] = 0.f;
					vertices[count * 8 + 7] = 0.f;

					count += 8;
				}

			} else {

				DBG_PRINT_MSG("Top-Right: %s", "next corner is a right-angle");

				vertices[count + 0] = maxx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = maxx + width;
				vertices[count + 1] = maxy;
				vertices[count + 2] = 1.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = maxx + width;
				vertices[count + 1] = maxy + width;
				vertices[count + 2] = 1.f;
				vertices[count + 3] = 1.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = maxx;
				vertices[count + 1] = maxy + width;
				vertices[count + 2] = (512 - 100) / 512.f;
				vertices[count + 3] = 1.f;
				count += 4;

			}

		}

		if(corner & RoundTopLeft) {

			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vertices[count * 8 + 0] = minx;
				vertices[count * 8 + 1] = 0.f;
				vertices[count * 8 + 2] = 0.f;
				vertices[count * 8 + 3] = 0.f;

				vertices[count * 8 + 4] = 0.f;
				vertices[count * 8 + 5] = 0.f;
				vertices[count * 8 + 6] = 0.f;
				vertices[count * 8 + 7] = 0.f;

				count += 8;
			}

		} else {

			if(corner & RoundBottomLeft) {	// the next corner is round?

				for(int i = 0; i < 3; i++) {

					vertices[count * 8 + 0] = 0.f;
					vertices[count * 8 + 1] = 0.f;
					vertices[count * 8 + 2] = 0.f;
					vertices[count * 8 + 3] = 0.f;

					vertices[count * 8 + 4] = 0.f;
					vertices[count * 8 + 5] = 0.f;
					vertices[count * 8 + 6] = 0.f;
					vertices[count * 8 + 7] = 0.f;

					count += 8;
				}

			} else {

				DBG_PRINT_MSG("Top-Left: %s", "next corner is a right-angle");

				vertices[count + 0] = minx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = maxy + width;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = 1.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = minx - width;
				vertices[count + 1] = maxy + width;
				vertices[count + 2] = 0.f;
				vertices[count + 3] = 1.f;
				count += 4;

				vertices[count + 0] = minx;
				vertices[count + 1] = maxy;
				vertices[count + 2] = 100 / 512.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

				vertices[count + 0] = minx - width;
				vertices[count + 1] = maxy;
				vertices[count + 2] = 0.f;
				vertices[count + 3] = (512 - 100) / 512.f;
				count += 4;

			}

		}

		vertices[count + 0] = vertices[0 + 0];
		vertices[count + 1] = vertices[0 + 1];
		vertices[count + 2] = vertices[0 + 2];
		vertices[count + 3] = vertices[0 + 3];
		count += 4;

		vertices[count + 0] = vertices[4 + 0];
		vertices[count + 1] = vertices[4 + 1];
		vertices[count + 2] = vertices[4 + 2];
		vertices[count + 3] = vertices[4 + 3];
		count += 4;

		assert(count == 104);

		//assert(count == (int)(total_vertex_number * 2 * 4));

	}

}
