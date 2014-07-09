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

#include <math.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	TabButton::TabButton ()
	: AbstractButton()
	{
		set_size(80, 14);
		set_checkable(true);

		InitializeTabButton();
	}

	TabButton::TabButton (const String& text)
	: AbstractButton()
	{
		set_size(80, 14);
		set_checkable(true);

		InitializeTabButton(text);
	}

	BlendInt::TabButton::~TabButton ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void TabButton::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if (request.target() == this) {
			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					std::vector<GLfloat> inner;
					std::vector<GLfloat> outer;
					GenerateTabButtonVertices(*size_p, DefaultBorderWidth(),
					        inner, outer);

					m_inner_buffer->Bind();
					m_inner_buffer->SetData(sizeof(GLfloat) * inner.size(),
					        &inner[0]);

					m_outer_buffer->Bind();
					m_outer_buffer->SetData(sizeof(GLfloat) * outer.size(),
					        &outer[0]);

					GLArrayBuffer::Reset();

					set_size(*size_p);
					Refresh();
					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	ResponseType TabButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glEnableVertexAttribArray(0);

		// draw inner, simple fill
		if (checked()) {
			program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
			program->SetUniform1i("AA", 0);

			glBindVertexArray(m_vao[0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 11);

		} else {
			program->SetVertexAttrib4fv("Color", Theme::instance->tab().item.data());
			program->SetUniform1i("AA", 1);

			glBindVertexArray(m_vao[0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 4, 2 * 11 - 4);
		}

		if (checked()) {
			program->SetUniform1i("AA", 1);
			program->SetVertexAttrib4fv("Color", Theme::instance->tab().outline.data());

			glBindVertexArray(m_vao[1]);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 11 * 2);
		}

		glBindVertexArray(0);
		program->Reset();

		if(text().size()) {
			font().Print(mvp, text(), text_length(), 0);
		}

		return Accept;
	}

	ResponseType TabButton::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	void TabButton::InitializeTabButton()
	{
		std::vector<GLfloat> inner;
		std::vector<GLfloat> outer;

		GenerateTabButtonVertices(size(), DefaultBorderWidth(), inner, outer);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		glGenVertexArrays(2, m_vao);

		glBindVertexArray(m_vao[0]);

		m_inner_buffer->Generate();
		m_inner_buffer->Bind();
		m_inner_buffer->SetData(sizeof(GLfloat) * inner.size(), &inner[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE,	0, 0);

		glBindVertexArray(m_vao[1]);

		m_outer_buffer->Generate();
		m_outer_buffer->Bind();
		m_outer_buffer->SetData(sizeof(GLfloat) * outer.size(), &outer[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE,	0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	void TabButton::InitializeTabButton(const String& text)
	{
	}
	
	void TabButton::GenerateTabButtonVertices (const Size& size, float border,
					std::vector<GLfloat>& inner, std::vector<GLfloat>& outer)
	{
		int amp = size.height() / 2;
		int shift_x = 5;
		border = Theme::instance->pixel() * border;

		if (inner.size() != 2 * 11 * 2)
			inner.resize(2 * 11 * 2);

		size_t count = 0;
		for (int i = 0; i <= 10; i++, count += 4) {
			inner[count + 0] = i;
			inner[count + 1] = sin_curve(i, amp, -shift_x, -border);
			inner[count + 2] = size.width() - i;
			inner[count + 3] = inner[count + 1];
		}

		if (outer.size() != 2 * 11 * 2 * 2)
			outer.resize(2 * 11 * 2 * 2);

		count = 0;
		for (int i = 0; i <= 10; i++, count += 4) {
			outer[count + 0] = i;
			outer[count + 1] = sin_curve(i, amp, -shift_x, 0.0);
			outer[count + 2] = i;
			outer[count + 3] = outer[count + 1] - border;
		}

		for (int i = 0; i <= 10; i++, count += 4) {
			outer[count + 0] = size.width() - outer[4 * (10 - i) + 0];
			outer[count + 1] = outer[4 * (10 - i) + 1];
			outer[count + 2] = size.width() - outer[4 * (10 - i) + 2];
			outer[count + 3] = outer[4 * (10 - i) + 3];
		}
	}

	inline double TabButton::sin_curve (double x, double amplitude,
					double shift_x, double shift_y)
	{
		return amplitude * sin((x + shift_x) / M_PI) + amplitude + shift_y;
	}
	
	Size TabButton::GetPreferredSize () const
	{
		return Size(80, 14);
	}

}

