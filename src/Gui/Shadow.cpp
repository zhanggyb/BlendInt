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

#include <math.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include <BlendInt/Gui/Shadow.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	static inline void copy_v2_v2(float r[2], const float a[2])
	{
		r[0] = a[0];
		r[1] = a[1];
	}

	Shadow::Shadow()
	: AbstractRoundForm(),
	  m_vao(0),
	  m_offset_x(0),
	  m_offset_y(0),
//	  m_direction(ShadowAll),
	  m_blur_rad(12.0)
	{
		set_size(90, 20);
		set_round_type(RoundAll);
		set_radius(5.0);
//		m_offset_x = 5;
//		m_offset_y = -5;

		InitializeShadow ();
	}

	Shadow::~Shadow()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Shadow::SetBlurRadius (float blur_rad)
	{
		if (blur_rad == m_blur_rad)
			return;

		//UpdateGeometry(UpdateRequest(ShadowBlurRadius, &blur_rad));

		m_blur_rad = blur_rad;
	}

	void Shadow::UpdateGeometry (const UpdateRequest& request)
	{
		switch (request.type()) {

			case FormSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				glBindVertexArray(m_vao);
				GenerateShadowBuffers(*size_p, radius(), m_blur_rad);

				glBindVertexArray(0);
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

	void Shadow::Draw (const glm::mat4& mvp, short gamma)
	{
		using Stock::Shaders;

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();
		program->Use();

		glm::mat4 offset = glm::translate(glm::mat4(1.0),
						glm::vec3(m_offset_x, m_offset_y, 0.f));

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
						glm::value_ptr(mvp * offset));
		program->SetUniform1i("AA", 0);
		program->SetUniform1i("Gamma", gamma);

		float alphastep;
		int step = 1;

		// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
//		const float radout = themes()->menu_shadow_width * 1.0;
//		alphastep = 3.0f * themes()->menu_shadow_fac / radout;
		alphastep = 2.0f * Theme::instance->shadow_fac() / m_blur_rad;

		float expfac = 0.0;

		glEnableVertexAttribArray(0);

		for (std::deque<RefPtr<GLArrayBuffer> >::iterator it =
						m_buffers.begin(); it != m_buffers.end(); it++) {
			expfac = sqrt(step / m_blur_rad);
			(*it)->Bind();

			glVertexAttribPointer(0, // attribute
							2,		// number of elements per vertex, here (x,y)
							GL_FLOAT,			 // the type of each element
							GL_FALSE,			 // take our values as-is
							0,			// no extra data between each position
							0					 // offset of first element
							);

			program->SetVertexAttrib4f("Color", 0.0f, 0.0f, 0.0f,
							alphastep * (1.0f - expfac));
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							(*it)->GetBufferSize() / (2 * sizeof(GLfloat)));
			step++;
		}

		GLArrayBuffer::Reset();
		glDisableVertexAttribArray(0);
		program->Reset();
		glBindVertexArray(0);
	}

	void Shadow::DrawAt(const glm::mat4& mvp, int x, int y)
	{
		glm::mat4 transed_mvp = glm::translate(mvp, glm::vec3(x, y, 0.f));
		Draw(transed_mvp);
	}

	void Shadow::DrawAt(const glm::mat4& mvp, const Point& pos)
	{
		glm::mat4 transed_mvp = glm::translate(mvp, glm::vec3(pos.x(), pos.y(), 0.f));
		Draw(transed_mvp);
	}

	void Shadow::InitializeShadow ()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		GenerateShadowBuffers(size(), radius(), m_blur_rad);

		glBindVertexArray(0);
	}

	int Shadow::generate_shadow_vertices (
			const Size* size,
			float rad,
			float step,
			float vert[WIDGET_SIZE_MAX][2])
	{
#ifdef DEBUG
		static bool debug_print = false;
#endif

		float vec[WIDGET_CURVE_RESOLU][2];
		float minx, miny, maxx, maxy;
		int i, tot = 0;

		rad += step;

		if (2.0f * rad > size->height())
			rad = 0.5f * size->height();

		minx = 0.0f - step;
		miny = 0.0f - step;
		maxx = size->width() + step;
		maxy = size->height() + step;

		/* mult */
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		/* start with left-top, anti clockwise */
		if (round_type() & RoundTopLeft) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx + rad - vec[i][0];
				vert[tot][1] = maxy - vec[i][1];
			}
		} else {
			vert[tot][0] = minx;
			vert[tot][1] = maxy;
			tot++;
		}

		if (round_type() & RoundBottomLeft) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx + vec[i][1];
				vert[tot][1] = miny + rad - vec[i][0];
			}
		} else {
			vert[tot][0] = minx;
			vert[tot][1] = miny;
			tot++;
		}

		if (round_type() & RoundBottomRight) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx - rad + vec[i][0];
				vert[tot][1] = miny + vec[i][1];
			}
		} else {
			vert[tot][0] = maxx;
			vert[tot][1] = miny;
			tot++;
		}

		if (round_type() & RoundTopRight) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx - vec[i][1];
				vert[tot][1] = maxy - rad + vec[i][0];
			}
		} else {
			vert[tot][0] = maxx;
			vert[tot][1] = maxy;
			tot++;
		}

#ifdef DEBUG
		if(debug_print) {

			int count = 0;

			std::cout << "-----------------------------------------------------------" << std::endl;

			std::cout << "total vertices: " << tot << std::endl;

			std::cout << "Top Left:" << std::endl;

			if(round_type() & RoundTopLeft) {
				for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
				{
					std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				}
			} else {
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				count++;
			}

			std::cout << "Bottom Left:" << std::endl;

			if(round_type() & RoundBottomLeft) {
				for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
				{
					std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				}
			} else {
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				count++;
			}

			std::cout << "Bottom Right:" << std::endl;

			if(round_type() & RoundBottomRight) {
				for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
				{
					std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				}
			}	else {
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				count++;
			}
			std::cout << "Top Right:" << std::endl;

			if(round_type() & RoundTopRight) {
				for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
				{
					std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				}
			} else {
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
				count++;
			}

//			debug_print = false;
		}
#endif

		return tot;
	}

	void Shadow::GenerateShadowVertices (const Size& size, int round_type, float radius, float step,
	        std::vector<GLfloat>& vertices)
	{
		float vec[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0f - step;
		float miny = 0.0f - step;
		float maxx = size.width() + step;
		float maxy = size.height() + step;

		radius += step;

		if (2.0f * radius > size.height())
			radius = 0.5f * size.height();

		/* mult */
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			vec[i][0] = radius * cornervec[i][0];
			vec[i][1] = radius * cornervec[i][1];
		}

		int count = 0;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		if(vertices.size() != outline_vertex_number * 2)
			vertices.resize(outline_vertex_number * 2);

		count = 0;

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[2 * count + 0] = minx + radius - vec[i][0];
				vertices[2 * count + 1] = maxy - vec[i][1];
			}
		} else {
			vertices[2 * count + 0] = minx;
			vertices[2 * count + 1] = maxy;
			count++;
		}

		if (round_type & RoundBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[2 * count + 0] = minx + vec[i][1];
				vertices[2 * count + 1] = miny + radius - vec[i][0];
			}
		} else {
			vertices[2 * count + 0] = minx;
			vertices[2 * count + 1] = miny;
			count++;
		}

		if (round_type & RoundBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[2 * count + 0] = maxx - radius + vec[i][0];
				vertices[2 * count + 1] = miny + vec[i][1];
			}
		} else {
			vertices[2 * count + 0] = maxx;
			vertices[2 * count + 1] = miny;
			count++;
		}

		if (round_type & RoundTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[2 * count + 0] = maxx - vec[i][1];
				vertices[2 * count + 1] = maxy - radius + vec[i][0];
			}
		} else {
			vertices[2 * count + 0] = maxx;
			vertices[2 * count + 1] = maxy;
			count++;
		}
	}

	void Shadow::GenerateShadowVerticesExt (const Size& size, int round_type,
	        float radius, float depth, std::vector<GLfloat>& vertices)
	{
		std::vector<GLfloat> vec1;	// the inner vertices
		std::vector<GLfloat> vec2;
		std::vector<GLfloat> vec3;

		float minx = 0.0f;
		float miny = 0.0f;
		float maxx = size.width();
		float maxy = size.height();

		float mid = depth / 2.f;

		if (2.0f * radius > size.height())
			radius = 0.5f * size.height();

		float rad1 = radius;
		float rad2 = radius + mid;
		float rad3 = radius + depth;

		int count = 0;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		vec1.resize(outline_vertex_number * 2);
		vec2.resize(outline_vertex_number * 2);
		vec3.resize(outline_vertex_number * 2);

		count = 0;

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vec1[count + 0] = minx + rad1 - rad1 * cornervec[i][0];
				vec1[count + 1] = maxy - rad1 * cornervec[i][1];

				vec2[count + 0] = minx + rad2 - rad2 * cornervec[i][0];
				vec2[count + 1] = maxy - rad2 * cornervec[i][1];

				vec3[count + 0] = minx + rad3 - rad3 * cornervec[i][0];
				vec3[count + 1] = maxy - rad3 * cornervec[i][1];

				count += 2;
			}
		} else {

			vec1[count + 0] = minx;
			vec1[count + 1] = maxy;

			vec2[count + 0] = minx;
			vec2[count + 1] = maxy;

			vec3[count + 0] = minx;
			vec3[count + 1] = maxy;

			count += 2;
		}

		if (round_type & RoundBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vec1[count + 0] = minx + rad1 * cornervec[i][1];
				vec1[count + 1] = miny + rad1 - rad1 * cornervec[i][0];

				vec2[count + 0] = minx + rad2 * cornervec[i][1];
				vec2[count + 1] = miny + rad2 - rad2 * cornervec[i][0];

				vec3[count + 0] = minx + rad3 * cornervec[i][1];
				vec3[count + 1] = miny + rad3 - rad3 * cornervec[i][0];

				count += 2;
			}
		} else {

			vec1[count + 0] = minx;
			vec1[count + 1] = miny;

			vec2[count + 0] = minx;
			vec2[count + 1] = miny;

			vec3[count + 0] = minx;
			vec3[count + 1] = miny;

			count += 2;
		}

		if (round_type & RoundBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vec1[count + 0] = maxx - rad1 + rad1 * cornervec[i][0];
				vec1[count + 1] = miny + rad1 * cornervec[i][1];

				vec2[count + 0] = maxx - rad2 + rad2 * cornervec[i][0];
				vec2[count + 1] = miny + rad2 * cornervec[i][1];

				vec3[count + 0] = maxx - rad3 + rad3 * cornervec[i][0];
				vec3[count + 1] = miny + rad3 * cornervec[i][1];

				count += 2;
			}
		} else {

			vec1[count + 0] = maxx;
			vec1[count + 1] = miny;

			vec2[count + 0] = maxx;
			vec2[count + 1] = miny;

			vec3[count + 0] = maxx;
			vec3[count + 1] = miny;

			count += 2;
		}

		if (round_type & RoundTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {

				vec1[count + 0] = maxx - rad1 * cornervec[i][1];
				vec1[count + 0] = maxy - rad1 + rad1 * cornervec[i][0];

				vec2[count + 0] = maxx - rad2 * cornervec[i][1];
				vec2[count + 0] = maxy - rad2 + rad2 * cornervec[i][0];

				vec3[count + 0] = maxx - rad3 * cornervec[i][1];
				vec3[count + 0] = maxy - rad3 + rad3 * cornervec[i][0];

				count += 2;
			}
		} else {

			vec1[count + 0] = maxx;
			vec1[count + 1] = maxy;

			vec2[count + 0] = maxx;
			vec2[count + 1] = maxy;

			vec3[count + 0] = maxx;
			vec3[count + 1] = maxy;

			count += 2;
		}

		assert((vec1.size() == vec2.size()) && (vec2.size() == vec3.size()));

		// now create the triangle strips
		if(vertices.size() != (outline_vertex_number + 1) * 6 * 2 * 2) {
			vertices.resize((outline_vertex_number + 1) * 6 * 2 * 2);
		}

		count = 0;
		for (unsigned int i = 0; i < outline_vertex_number;)
		{
			vertices[count + 0] = vec1[i + 0];
			vertices[count + 1] = vec1[i + 1];

			vertices[count + 2] = 0.25f;
			vertices[count + 3] = 0.25f;
			vertices[count + 4] = 0.25f;
			vertices[count + 5] = 0.75f;

			vertices[count + 6] = vec2[i + 0];
			vertices[count + 7] = vec2[i + 1];

			vertices[count + 8] = 0.25f;
			vertices[count + 9] = 0.25f;
			vertices[count + 10] = 0.25f;
			vertices[count + 11] = 0.25f;

			i = i + 2;
			count = count + 12;
		}

		vertices[count + 0] = vec1[0];
		vertices[count + 1] = vec1[1];

		vertices[count + 2] = 0.25f;
		vertices[count + 3] = 0.25f;
		vertices[count + 4] = 0.25f;
		vertices[count + 5] = 0.75f;

		vertices[count + 6] = vec2[0];
		vertices[count + 7] = vec2[1];

		vertices[count + 8] = 0.25f;
		vertices[count + 9] = 0.25f;
		vertices[count + 10] = 0.25f;
		vertices[count + 11] = 0.25f;

		count = count + 12;

		for (unsigned int i = 0; i < outline_vertex_number;)
		{
			vertices[count + 0] = vec2[i + 0];
			vertices[count + 1] = vec2[i + 1];

			vertices[count + 2] = 0.25f;
			vertices[count + 3] = 0.25f;
			vertices[count + 4] = 0.25f;
			vertices[count + 5] = 0.25f;

			vertices[count + 6] = vec3[i + 0];
			vertices[count + 7] = vec3[i + 1];

			vertices[count + 8] = 0.25f;
			vertices[count + 9] = 0.25f;
			vertices[count + 10] = 0.25f;
			vertices[count + 11] = 0.025f;

			i = i + 2;
			count = count + 12;
		}

		vertices[count + 0] = vec2[0];
		vertices[count + 1] = vec2[1];

		vertices[count + 2] = 0.25f;
		vertices[count + 3] = 0.25f;
		vertices[count + 4] = 0.25f;
		vertices[count + 5] = 0.25f;

		vertices[count + 6] = vec3[0];
		vertices[count + 7] = vec3[1];

		vertices[count + 8] = 0.25f;
		vertices[count + 9] = 0.25f;
		vertices[count + 10] = 0.25f;
		vertices[count + 11] = 0.025f;

	}

	void Shadow::verts_to_quad_strip (const float inner_v[WIDGET_SIZE_MAX][2],
					const float outer_v[WIDGET_SIZE_MAX][2], const int totvert,
					float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int i = 0;
		for (; i < totvert; i++) {
			copy_v2_v2(quad_strip[i * 2], outer_v[i]);
			copy_v2_v2(quad_strip[i * 2 + 1], inner_v[i]);
		}
		copy_v2_v2(quad_strip[i * 2], outer_v[0]);
		copy_v2_v2(quad_strip[i * 2 + 1], inner_v[0]);
	}

	void Shadow::GenerateShadowBuffers(const Size& size, float corner_rad, float blur_rad)
	{
		Size shadow_size = size;

		float inner_v[WIDGET_SIZE_MAX][2];
		float outer_v[WIDGET_SIZE_MAX][2];

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];

		//const float radout = Theme::instance->menu_shadow_width() * 1.0;
		const float radout = 16.0;

		/* prevent tooltips to not show round shadow */
//		if (radout > 0.2f * size.height())
//			shadow_size.add_height(-0.2f * size.height());
//		else
//			shadow_size.add_height(-radout);

		int totvert = 0;

		m_buffers.clear();

		totvert = generate_shadow_vertices(&shadow_size, radius(), 0.0f,
		        inner_v);

		for (int step = 1; step <= (int) blur_rad; step++) {

			generate_shadow_vertices(&shadow_size, corner_rad, (float) step,
			        outer_v);
			verts_to_quad_strip(inner_v, outer_v, totvert, quad_strip);

			RefPtr<GLArrayBuffer> buffer(new GLArrayBuffer);
			buffer->Generate();
			buffer->Bind();
			buffer->SetData((totvert * 2 + 2) * sizeof(quad_strip[0]), quad_strip);
			buffer->Reset();

			m_buffers.push_back(buffer);
		}
	}

}

