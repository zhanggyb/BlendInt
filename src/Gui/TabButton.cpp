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
	: AbstractButton(), m_vao(0)
	{
		set_size(80, 14);
		set_checkable(true);

		InitializeTabButton();
	}

	TabButton::TabButton (const String& text)
	: AbstractButton(), m_vao(0)
	{
		set_size(80, 14);
		set_checkable(true);

		InitializeTabButton(text);
	}

	BlendInt::TabButton::~TabButton ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void TabButton::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				/*
				 GenerateFormBuffer(*size_p,
				 RoundNone,
				 0.f,
				 m_inner_buffer.get(),
				 m_outer_buffer.get(),
				 0);
				 glBindVertexArray(0);
				 */
				GenerateTabButtonBuffers(*size_p, m_inner_buffer.get(),
								m_outer_buffer.get());

				Refresh();
				break;
			}

			default:
				break;
		}

	}

	ResponseType TabButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		Theme* tm = Theme::instance;

		glEnableVertexAttribArray(0);

		// draw inner, simple fill
		if (checked()) {
			program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
			program->SetUniform1i("AA", 0);
			DrawTriangleStrip(0, m_inner_buffer.get());
		} else {
			program->SetVertexAttrib4fv("Color", tm->tab().item.data());
			program->SetUniform1i("AA", 1);
			m_inner_buffer->Bind();
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			// Skip the bottom triangle strip for better appearance
			glDrawArrays(GL_TRIANGLE_STRIP, 4,
							m_inner_buffer->GetBufferSize()
												/ (2 * sizeof(GLfloat)) - 4);
			m_inner_buffer->Reset();
		}

		if (checked()) {
			program->SetUniform1i("AA", 1);
			program->SetVertexAttrib4fv("Color", Theme::instance->tab().outline.data());
			DrawTriangleStrip(0, m_outer_buffer.get());
		}

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

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
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		/*
		GenerateFormBuffer(
						size(),
						RoundNone,
						0.f,
						m_inner_buffer.get(),
						m_outer_buffer.get(),
						0);
						*/
		GenerateTabButtonBuffers(size(), m_inner_buffer.get(), m_outer_buffer.get());
		glBindVertexArray(0);
	}

	void TabButton::InitializeTabButton(const String& text)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindVertexArray(0);
	}
	
	void TabButton::GenerateTabButtonVertices (const Size& size, float border,
					std::vector<GLfloat>* inner, std::vector<GLfloat>* outer)
	{
		if((!inner) && (!outer))
			return;

		int amp = size.height() / 2;
		int shift_x = 5;

		if(inner) {

			if(inner->size() != 2 * 11 * 2)
				inner->resize(2 * 11 * 2);

			size_t count = 0;
			for(int i = 0; i <= 10; i++, count += 4)
			{
				(*inner)[count + 0] = i;
				(*inner)[count + 1] = sin_curve(i, amp, -shift_x, -border);
				(*inner)[count + 2] = size.width() - i;
				(*inner)[count + 3] = (*inner)[count + 1];
			}

		}

		if(outer) {

			if(outer->size() != 2 * 11 * 2 * 2)
				outer->resize(2 * 11 * 2 * 2);

			size_t count = 0;
			for(int i = 0; i <= 10; i++, count += 4)
			{
				(*outer)[count + 0] = i;
				(*outer)[count + 1] = sin_curve(i, amp, -shift_x, 0.0);
				(*outer)[count + 2] = i;
				(*outer)[count + 3] = (*outer)[count + 1] - border;
			}

			for(int i = 0; i <= 10; i++, count += 4)
			{
				(*outer)[count + 0] = size.width() - (*outer)[4 * (10 - i) + 0];
				(*outer)[count + 1] = (*outer)[4 * (10 - i) + 1];
				(*outer)[count + 2] = size.width() - (*outer)[4 * (10 - i) + 2];
				(*outer)[count + 3] = (*outer)[4 * (10 - i) + 3];
			}
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

	void TabButton::GenerateTabButtonBuffers (const Size& size, GLArrayBuffer* inner_buffer,
					GLArrayBuffer* outer_buffer)
	{
		std::vector<GLfloat> inner_vertices;
		std::vector<GLfloat> outer_vertices;

		GenerateTabButtonVertices(size, 1.f, &inner_vertices, &outer_vertices);

		if(inner_buffer) {
			if(!inner_buffer->IsBuffer()) {
				inner_buffer->Generate();
			}
			inner_buffer->Bind();
			inner_buffer->SetData(sizeof(GLfloat) * inner_vertices.size(),
							&inner_vertices[0]);
			inner_buffer->Reset();
		}

		if(outer_buffer) {
			if(!outer_buffer->IsBuffer()) {
				outer_buffer->Generate();
			}
			outer_buffer->Bind();
			outer_buffer->SetData(sizeof(GLfloat) * outer_vertices.size(),
							&outer_vertices[0]);
			outer_buffer->Reset();
		}

	}

}

