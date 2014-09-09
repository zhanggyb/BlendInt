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

	Shadow::Shadow(const Size& s, int t, float r)
	: AbstractRoundForm(),
	  m_vao(0)
	{
		set_size(s);
		set_round_type(t);
		set_radius(r);

		InitializeShadow ();
	}

	Shadow::~Shadow()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Shadow::UpdateGeometry (const UpdateRequest& request)
	{
		switch (request.type()) {

			case FormSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVerticesExt(*size_p, round_type(), radius(), vertices);
				m_buffer->bind();
				m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				m_buffer->reset();

				break;
			}

			case FormRoundType: {

				const int* type_p = static_cast<const int*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVerticesExt(size(), *type_p, radius(), vertices);
				m_buffer->bind();
				m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				m_buffer->reset();

				break;
			}

			case FormRoundRadius: {

				const float* radius_p = static_cast<const float*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVerticesExt(size(), round_type(), *radius_p, vertices);
				m_buffer->bind();
				m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				m_buffer->reset();

				break;
			}

			/*
			case ShadowBlurRadius: {

				const float* blur_rad =
								static_cast<const float*>(request.data());

				glBindVertexArray(m_vao);
				GenerateShadowBuffers(size(), radius(), *blur_rad);
				glBindVertexArray(0);
				break;
			}
			*/

			default:
				break;
		}

	}

	void Shadow::Draw (const glm::vec3& pos, short gamma)
	{
		RefPtr<GLSLProgram> program =
				Shaders::instance->triangle_program();
		program->use();

		glUniform3fv(Shaders::instance->triangle_uniform_position(), 1,
		        glm::value_ptr(pos));
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), gamma);

		// fine tune the shadow alpha, default is 10.0 * 0.5 / 12.0
		float alphastep = 10.0f * Theme::instance->shadow_fac()
		        / Theme::instance->shadow_width();
		float expfac = 0.f;
		int verts = GetOutlineVertices(round_type());
		verts = verts * 2 + 2;

		// the first circle use anti-alias
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.f, 0.f, 0.f, alphastep);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, verts);

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);
		for (int i = 1; i < Theme::instance->shadow_width(); i++) {
			expfac = sqrt(i / (float) Theme::instance->shadow_width());

			glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.f,
			        0.f, 0.f, alphastep * (1.0f - expfac));
			glDrawArrays(GL_TRIANGLE_STRIP, verts * i, verts);
		}

		glBindVertexArray(0);

		GLArrayBuffer::reset();
		program->reset();
	}

	void Shadow::InitializeShadow ()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> vertices;
		GenerateShadowVerticesExt(size(), round_type(), radius(), vertices);

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->generate();
		m_buffer->bind();
		m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
		        GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void Shadow::Update (int width, int height, int type, float rad)
	{
		set_size(width, height);
		set_round_type(type);
		set_radius(rad);

		std::vector<GLfloat> vertices;
		GenerateShadowVerticesExt(size(), this->round_type(), this->radius(), vertices);
		m_buffer->bind();
		m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_buffer->reset();
	}

	void Shadow::Update (const Size& new_size, int type, float rad)
	{
		if((new_size == size()) && (type == round_type()) && (rad == radius()))
			return;

		set_size(new_size);
		set_round_type(type);
		set_radius(rad);

		std::vector<GLfloat> vertices;
		GenerateShadowVerticesExt(size(), this->round_type(), this->radius(), vertices);
		m_buffer->bind();
		m_buffer->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_buffer->reset();
	}

	void Shadow::GenerateShadowVerticesExt (const Size& size, int round_type,
	        float radius, std::vector<GLfloat>& vertices)
	{
		float minx = 0.0f;
		float miny = 0.0f;
		float maxx = size.width();
		float maxy = size.height();

		if (2.0f * radius > size.height())
			radius = 0.5f * size.height();

		int count = 0;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int edge_vertex_count = 4 - count + count * WIDGET_CURVE_RESOLU;

		unsigned int max_verts = (edge_vertex_count + 1) * 2 * 2 * Theme::instance->shadow_width();

		//DBG_PRINT_MSG("max verts: %u", max_verts);

		if(vertices.size() != max_verts) {
			vertices.resize(max_verts);
		}

		float radi = 0.f;
		float rado = 0.f;
		count = 0;

		for(int i = 0; i < Theme::instance->shadow_width(); i++) {

			radi = radius + i;
			rado = radi + 1.f;

			/* start with left-top, anti clockwise */
			if (round_type & RoundTopLeft) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

					vertices[count + 0] = minx - i + radi - radi * cornervec[j][0];
					vertices[count + 1] = maxy + i - radi * cornervec[j][1];

					vertices[count + 2] = minx - (i + 1) + rado - rado * cornervec[j][0];
					vertices[count + 3] = maxy + (i + 1) - rado * cornervec[j][1];

					count += 4;
				}
			} else {

				vertices[count + 0] = minx - i;
				vertices[count + 1] = maxy + i;

				vertices[count + 2] = minx - (i + 1);
				vertices[count + 3] = maxy + (i + 1);

				count += 4;
			}

			if (round_type & RoundBottomLeft) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

					vertices[count + 0] = minx - i + radi * cornervec[j][1];
					vertices[count + 1] = miny - i + radi - radi * cornervec[j][0];

					vertices[count + 2] = minx - (i + 1) + rado * cornervec[j][1];
					vertices[count + 3] = miny - (i + 1)+ rado - rado * cornervec[j][0];

					count += 4;
				}
			} else {

				vertices[count + 0] = minx - i;
				vertices[count + 1] = miny - i;

				vertices[count + 2] = minx - (i + 1);
				vertices[count + 3] = miny - (i + 1);

				count += 4;
			}

			if (round_type & RoundBottomRight) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

					vertices[count + 0] = maxx + i - radi + radi * cornervec[j][0];
					vertices[count + 1] = miny - i + radi * cornervec[j][1];

					vertices[count + 2] = maxx + (i + 1) - rado + rado * cornervec[j][0];
					vertices[count + 3] = miny - (i + 1) + rado * cornervec[j][1];

					count += 4;
				}
			} else {

				vertices[count + 0] = maxx + i;
				vertices[count + 1] = miny - i;

				vertices[count + 2] = maxx + (i + 1);
				vertices[count + 3] = miny - (i + 1);

				count += 4;
			}

			if (round_type & RoundTopRight) {
				for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

					vertices[count + 0] = maxx + i - radi * cornervec[j][1];
					vertices[count + 1] = maxy + i - radi + radi * cornervec[j][0];

					vertices[count + 2] = maxx + (i + 1) - rado * cornervec[j][1];
					vertices[count + 3] = maxy + (i + 1) - rado + rado * cornervec[j][0];

					count += 4;
				}
			} else {

				vertices[count + 0] = maxx + i;
				vertices[count + 1] = maxy + i;

				vertices[count + 2] = maxx + (i + 1);
				vertices[count + 3] = maxy + (i + 1);

				count += 4;
			}

			vertices[count + 0] = vertices[count - edge_vertex_count * 4];
			vertices[count + 1] = vertices[count - edge_vertex_count * 4 + 1];

			vertices[count + 2] = vertices[count - edge_vertex_count * 4 + 2];
			vertices[count + 3] = vertices[count - edge_vertex_count * 4 + 3];

			//DBG_PRINT_MSG("count: %d", count);

			count += 4;

			//radi = radi + (float)i;
		}

	}

	// ----------------------------------------------

	ShadowExt::ShadowExt()
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(100, 100);

		InitializeShadow();
	}

	ShadowExt::ShadowExt(const Size& size, int round_type, float radius)
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(size);
		set_round_type(round_type);
		set_radius(radius);

		InitializeShadow();
	}

	ShadowExt::~ShadowExt()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ShadowExt::Draw (const glm::vec3& pos, short gamma)
	{
		RefPtr<GLSLProgram> program =
				Shaders::instance->widget_program();
		program->use();

		int count = GetOutlineVertices(round_type());
		//count = count * 2 + 2;

		glUniform3fv(Shaders::instance->location(Stock::WIDGET_POSITION), 1,
		        glm::value_ptr(pos));
		glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), gamma);
		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.f,
		        0.f, 1.f, 0.75f);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 1);

		glBindVertexArray(vao_);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, (count + 1) * 2 * 2);

		//glDrawArrays(GL_TRIANGLE_STRIP, (count + 1) * 3 * 2, (count + 1) * 2);

		glBindVertexArray(0);

		GLArrayBuffer::reset();
		program->reset();
	}

	void ShadowExt::UpdateGeometry(const UpdateRequest& request)
	{
		switch (request.type()) {

			case FormSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVertices(*size_p, round_type(), radius(), vertices);
				buffer_->bind();
				buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				buffer_->reset();

				break;
			}

			case FormRoundType: {

				const int* type_p = static_cast<const int*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVertices(size(), *type_p, radius(), vertices);
				buffer_->bind();
				buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				buffer_->reset();

				break;
			}

			case FormRoundRadius: {

				const float* radius_p = static_cast<const float*>(request.data());

				std::vector<GLfloat> vertices;
				GenerateShadowVertices(size(), round_type(), *radius_p, vertices);
				buffer_->bind();
				buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				buffer_->reset();

				break;
			}

			default:
				break;
		}

	}

	void ShadowExt::InitializeShadow()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius(), vertices);

		buffer_.reset(new GLArrayBuffer);
		buffer_->generate();
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD), 3,
		        GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void ShadowExt::GenerateShadowVertices(const Size& size, int round_type,
			float radius, std::vector<GLfloat>& vertices)
	{
		float minx = 0.0f;
		float miny = 0.0f;
		float maxx = size.width();
		float maxy = size.height();

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

		int count = 0;
		int edge_vertex_count = GetOutlineVertices(round_type);

		unsigned int max_count = (edge_vertex_count + 1) * 3 * 2 * 2;

		//DBG_PRINT_MSG("max verts: %u", max_verts);

		if(vertices.size() != max_count) {
			vertices.resize(max_count);
		}

		count = 0;

		float w = Theme::instance->shadow_width() / 3.f;
		float radi = radius;
		float rado = radi + Theme::instance->shadow_width() / 3.f;

		// TODO: find tune shade 1 ~ 3
		float shade1 = -7 / 255.f;
		float shade2 = -19 / 255.f;
		float shade3 = -25.5 / 255.f;

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi - radi * cornervec[j][0];
				vertices[count + 1] = maxy - radi * cornervec[j][1];
				vertices[count + 2] = shade1;

				vertices[count + 3] = minx - w + rado - rado * cornervec[j][0] + x_offset / 3.f;
				vertices[count + 4] = maxy + w - rado * cornervec[j][1] + y_offset / 3.f;
				vertices[count + 5] = shade2;

				count += 6;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = maxy;
			vertices[count + 2] = shade1;

			vertices[count + 3] = minx - w + x_offset / 3.f;
			vertices[count + 4] = maxy + w + y_offset / 3.f;
			vertices[count + 5] = shade2;

			count += 6;
		}

		if (round_type & RoundBottomLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi * cornervec[j][1];
				vertices[count + 1] = miny + radi - radi * cornervec[j][0];
				vertices[count + 2] = shade1;

				vertices[count + 3] = minx - w + rado * cornervec[j][1] + x_offset / 3.f;
				vertices[count + 4] = miny - w + rado - rado * cornervec[j][0] + y_offset / 3.f;
				vertices[count + 5] = shade2;

				count += 6;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = miny;
			vertices[count + 2] = shade1;

			vertices[count + 3] = minx - w + x_offset / 3.f;
			vertices[count + 4] = miny - w + y_offset / 3.f;
			vertices[count + 5] = shade2;

			count += 6;
		}

		if (round_type & RoundBottomRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi + radi * cornervec[j][0];
				vertices[count + 1] = miny + radi * cornervec[j][1];
				vertices[count + 2] = shade1;

				vertices[count + 3] = maxx + w - rado + rado * cornervec[j][0] + x_offset / 3.f;
				vertices[count + 4] = miny - w + rado * cornervec[j][1] + y_offset / 3.f;
				vertices[count + 5] = shade2;

				count += 6;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = miny;
			vertices[count + 2] = shade1;

			vertices[count + 3] = maxx + w + x_offset / 3.f;
			vertices[count + 4] = miny - w + y_offset / 3.f;
			vertices[count + 5] = shade2;

			count += 6;
		}

		if (round_type & RoundTopRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi * cornervec[j][1];
				vertices[count + 1] = maxy - radi + radi * cornervec[j][0];
				vertices[count + 2] = shade1;

				vertices[count + 3] = maxx + w - rado * cornervec[j][1] + x_offset / 3.f;
				vertices[count + 4] = maxy + w - rado + rado * cornervec[j][0] + y_offset / 3.f;
				vertices[count + 5] = shade2;

				count += 6;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = maxy;
			vertices[count + 2] = shade1;

			vertices[count + 3] = maxx + w + x_offset / 3.f;
			vertices[count + 4] = maxy + w + y_offset / 3.f;
			vertices[count + 5] = shade2;

			count += 6;
		}

		vertices[count + 0] = vertices[count - edge_vertex_count * 6];
		vertices[count + 1] = vertices[count - edge_vertex_count * 6 + 1];
		vertices[count + 2] = vertices[count - edge_vertex_count * 6 + 2];

		vertices[count + 3] = vertices[count - edge_vertex_count * 6 + 3];
		vertices[count + 4] = vertices[count - edge_vertex_count * 6 + 4];
		vertices[count + 5] = vertices[count - edge_vertex_count * 6 + 5];

		count += 6;

		minx = minx - w + x_offset / 3.f;
		miny = miny - w + y_offset / 3.f;
		maxx = maxx + w + x_offset / 3.f;
		maxy = maxy + w + y_offset / 3.f;

		w = Theme::instance->shadow_width() / 3.f * 2.f;

		radi = rado;
		rado = radi + Theme::instance->shadow_width();

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi - radi * cornervec[j][0];
				vertices[count + 1] = maxy - radi * cornervec[j][1];
				vertices[count + 2] = shade2;

				vertices[count + 3] = minx - w + rado - rado * cornervec[j][0] + x_offset / 1.f;
				vertices[count + 4] = maxy + w - rado * cornervec[j][1] + y_offset / 1.f + y_offset / 1.f;
				vertices[count + 5] = shade3;

				count += 6;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = maxy;
			vertices[count + 2] = shade2;

			vertices[count + 3] = minx - w + x_offset / 1.f;
			vertices[count + 4] = maxy + w + y_offset / 1.f;
			vertices[count + 5] = shade3;

			count += 6;
		}

		if (round_type & RoundBottomLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi * cornervec[j][1];
				vertices[count + 1] = miny + radi - radi * cornervec[j][0];
				vertices[count + 2] = shade2;

				vertices[count + 3] = minx - w + rado * cornervec[j][1] + x_offset / 1.f;
				vertices[count + 4] = miny - w + rado - rado * cornervec[j][0] + y_offset / 1.f;
				vertices[count + 5] = shade3;

				count += 6;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = miny;
			vertices[count + 2] = shade2;

			vertices[count + 3] = minx - w + x_offset / 1.f;
			vertices[count + 4] = miny - w + y_offset / 1.f;
			vertices[count + 5] = shade3;

			count += 6;
		}

		if (round_type & RoundBottomRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi + radi * cornervec[j][0];
				vertices[count + 1] = miny + radi * cornervec[j][1];
				vertices[count + 2] = shade2;

				vertices[count + 3] = maxx + w - rado + rado * cornervec[j][0] + x_offset / 1.f;
				vertices[count + 4] = miny - w + rado * cornervec[j][1] + y_offset / 1.f;
				vertices[count + 5] = shade3;

				count += 6;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = miny;
			vertices[count + 2] = shade2;

			vertices[count + 3] = maxx + w + x_offset / 1.f;
			vertices[count + 4] = miny - w + y_offset / 1.f;
			vertices[count + 5] = shade3;

			count += 6;
		}

		if (round_type & RoundTopRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi * cornervec[j][1];
				vertices[count + 1] = maxy - radi + radi * cornervec[j][0];
				vertices[count + 2] = shade2;

				vertices[count + 3] = maxx + w - rado * cornervec[j][1] + x_offset / 1.f;
				vertices[count + 4] = maxy + w - rado + rado * cornervec[j][0] + y_offset / 1.f;
				vertices[count + 5] = shade3;

				count += 6;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = maxy;
			vertices[count + 2] = shade2;

			vertices[count + 3] = maxx + w + x_offset / 1.f;
			vertices[count + 4] = maxy + w + y_offset / 1.f;
			vertices[count + 5] = shade3;

			count += 6;
		}

		vertices[count + 0] = vertices[count - edge_vertex_count * 6];
		vertices[count + 1] = vertices[count - edge_vertex_count * 6 + 1];
		vertices[count + 2] = vertices[count - edge_vertex_count * 6 + 2];

		vertices[count + 3] = vertices[count - edge_vertex_count * 6 + 3];
		vertices[count + 4] = vertices[count - edge_vertex_count * 6 + 4];
		vertices[count + 5] = vertices[count - edge_vertex_count * 6 + 5];

		count += 6;

	}

}
