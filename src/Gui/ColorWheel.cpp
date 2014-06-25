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
#endif	// __UNIX__

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	ColorWheel::ColorWheel()
	: AbstractWidget(),
	  m_vao(0)
	{
		set_size(160, 160);

		m_slide_icon.Resize(3);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> inner_vertices;
		std::vector<GLfloat> outer_vertices;

		GenerateWheelVertices(80, inner_vertices, outer_vertices);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();

		m_inner->Bind();
		m_inner->SetData(sizeof(GLfloat) * inner_vertices.size(), &inner_vertices[0], GL_STATIC_DRAW);
		m_inner->Reset();

		m_outline.reset(new GLArrayBuffer);
		m_outline->Generate();
		m_outline->Bind();
		m_outline->SetData(sizeof(GLfloat) * outer_vertices.size(), &outer_vertices[0], GL_STATIC_DRAW);
		m_outline->Reset();

		glBindVertexArray(0);
	}
	
	ColorWheel::~ColorWheel ()
	{
		glDeleteVertexArrays(1, &m_vao);
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

	ResponseType ColorWheel::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) (position().x() + size().width() / 2),
		        (float) (position().y() + size().height() / 2), (float) z());

		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);
		RefPtr<GLSLProgram> program =
						Shaders::instance->default_triangle_program();
		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		m_inner->Bind();
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4,	GL_FLOAT, GL_FALSE,	sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));
		glDrawArrays(GL_TRIANGLE_FAN, 0, m_inner->GetBufferSize() / (6 * sizeof(GLfloat)));
		m_inner->Reset();

		glDisableVertexAttribArray(1);

		Color color = Theme::instance->regular().outline;
		program->SetVertexAttrib4fv("Color", color.data());
		program->SetUniform1i("AA", 1);
		m_outline->Bind();
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
						m_outline->GetBufferSize()
						/ (2 * sizeof(GLfloat)));
		m_outline->Reset();

		glDisableVertexAttribArray(0);
		program->Reset();
		glBindVertexArray(0);

		glm::mat4 icon_mvp;

		icon_mvp = glm::translate(mvp, glm::vec3(8.f, 12.f, 0.f));

		m_slide_icon.Draw(icon_mvp);

		return Accept;
	}
	
	void ColorWheel::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		switch(request.type()) {

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				int radius = std::min(size_p->width(), size_p->height()) / 2;

				m_inner->Bind();

				GLfloat* ptr = (GLfloat*) m_inner->Map(GL_READ_WRITE);

				double rad = 0.0;
				float x1 = 0.f;
				float y1 = 0.f;

				ptr = ptr + 6;
				int i = 1;
				for(int angle = -30; angle < 330; angle = angle + 5)
				{
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

				m_inner->Unmap();
				m_inner->Reset();

				m_outline->Bind();

				ptr = (GLfloat*) m_outline->Map(GL_READ_WRITE);
				float x2 = 0.f;
				float y2 = 0.f;

				i = 0;
				for(int angle = -30; angle < 330; angle = angle + 5)
				{
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

				m_outline->Unmap();
				m_outline->Reset();

				break;
			}

			default:
				break;

		}
	}

	bool ColorWheel::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		return true;
	}

	void ColorWheel::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
	}

	ResponseType ColorWheel::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ColorWheel::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ColorWheel::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ColorWheel::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ColorWheel::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ColorWheel::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ColorWheel::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ColorWheel::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
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

}
