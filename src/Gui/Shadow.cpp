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
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

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

		InitOnce ();
	}

	Shadow::~Shadow()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Shadow::SetBlurRadius (float blur_rad)
	{
		if (blur_rad == m_blur_rad)
			return;

		Update(UpdateRequest(Predefined, ShadowBlurRadius, &blur_rad));

		m_blur_rad = blur_rad;
	}

	void Shadow::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {
			switch (request.type()) {

				case FormSize: {

					const Size* size_p =
									static_cast<const Size*>(request.data());
					glBindVertexArray(m_vao);
					GenerateShadowBuffers(*size_p, radius(), m_blur_rad);

					glBindVertexArray(0);
					break;
				}

				case ShadowBlurRadius: {

					const float* blur_rad =
									static_cast<const float*>(request.data());

					glBindVertexArray(m_vao);
					GenerateShadowBuffers(size(), radius(), *blur_rad);
					glBindVertexArray(0);
					break;
				}

				default:
					break;
			}

		}
	}

	void Shadow::Draw (const glm::mat4& mvp)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		glm::mat4 offset = glm::translate(glm::mat4(1.0),
						glm::vec3(m_offset_x, m_offset_y, 0.f));

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
						glm::value_ptr(mvp * offset));
		program->SetUniform1i("AA", 0);
		program->SetUniform1i("Gamma", 0);

		float alphastep;
		int step = 1;

		// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
//		const float radout = themes()->menu_shadow_width * 1.0;
//		alphastep = 3.0f * themes()->menu_shadow_fac / radout;
		alphastep = 3.0f * themes()->menu_shadow_fac / m_blur_rad;

		float expfac = 0.0;

		glEnableVertexAttribArray(0);

		for (std::vector<RefPtr<GLArrayBuffer> >::iterator it =
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
			(*it)->Reset();
			step++;
		}

		glDisableVertexAttribArray(0);
		program->Reset();
		glBindVertexArray(0);
	}

	void Shadow::InitOnce ()
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

	void Shadow::GenerateShadowBuffers(const Size& size, float corner_rad, float blur_rad)
	{
		Size shadow_size = size;

		float inner_v[WIDGET_SIZE_MAX][2];
		float outer_v[WIDGET_SIZE_MAX][2];

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];

		const float radout = themes()->menu_shadow_width * 1.0;

		/* prevent tooltips to not show round shadow */
		if (radout > 0.2f * size.height())
			shadow_size.add_height(-0.2f * size.height());
		else
			shadow_size.add_height(-radout);

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
