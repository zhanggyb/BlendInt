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
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	TabButton::TabButton ()
	: AbstractButton(), m_vao(0)
	{
		set_size(80, 20);
		set_preferred_size(80, 20);

		InitOnce();
	}

	TabButton::TabButton (const String& text)
	: AbstractButton(), m_vao(0)
	{
		set_size(80, 20);
		set_preferred_size(80, 20);

		InitOnce(text);
	}

	BlendInt::TabButton::~TabButton ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool TabButton::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					glBindVertexArray(m_vao);
					GenerateFormBuffer(*size_p,
									   RoundNone,
									   0.f,
									   m_inner_buffer.get(),
									   m_outer_buffer.get(),
									   0);
					glBindVertexArray(0);
					Refresh();
					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType TabButton::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		ThemeManager* tm = ThemeManager::instance();

		glm::vec4 color;

		// draw inner, simple fill
		if(down()) {
			color.r = tm->themes()->regular.inner_sel.r() / 255.f;
			color.g = tm->themes()->regular.inner_sel.g() / 255.f;
			color.b = tm->themes()->regular.inner_sel.b() / 255.f;
			color.a = tm->themes()->regular.inner_sel.a() / 255.f;
		} else {
			if(hover()) {
				color.r = tm->themes()->regular.inner.highlight_red() / 255.f;
				color.g = tm->themes()->regular.inner.highlight_green() / 255.f;
				color.b = tm->themes()->regular.inner.highlight_blue() / 255.f;
				color.a = tm->themes()->regular.inner.a() / 255.f;
			} else {
				color.r = tm->themes()->regular.inner.r() / 255.f;
				color.g = tm->themes()->regular.inner.g() / 255.f;
				color.b = tm->themes()->regular.inner.b() / 255.f;
				color.a = tm->themes()->regular.inner.a() / 255.f;
			}
		}

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		glEnableVertexAttribArray(0);

		//DrawTriangleFan(0, m_inner_buffer.get());

		color.r = themes()->regular.outline.r() / 255.f;
		color.g = themes()->regular.outline.g() / 255.f;
		color.b = themes()->regular.outline.b() / 255.f;
		color.a = themes()->regular.outline.a() / 255.f;

		program->SetUniform1i("AA", 1);

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		DrawTriangleStrip(0, m_outer_buffer.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		if(text().size()) {
			font().Print(mvp, origin().x(), origin().y(), text(), text_length(), 0);
		}

		return Accept;
	}

	void TabButton::InitOnce()
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
		GenerateBuffers(size(), 0, m_outer_buffer.get());

		glBindVertexArray(0);
	}

	void TabButton::InitOnce(const String& text)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindVertexArray(0);
	}
	
	void TabButton::GenerateVertices (const Size& size, float border,
					std::vector<GLfloat>* inner, std::vector<GLfloat>* outer)
	{
		if((!inner) && (!outer))
			return;

		int hh = size.height() / 2;
		int hw = 10;

		int stride = size.width() - 2 * hw;

		if(outer) {

			if(outer->size() != 2 * 11 * 2)
				outer->resize(2 * 11 * 2);

			for(int i = 0; i <= 10; i++)
			{
				(*outer)[4 * i + 0] = i;
				(*outer)[4 * i + 1] = hh * sin((i - hw) / (2 * M_PI)) + hh;
				(*outer)[4 * i + 2] = i;
				(*outer)[4 * i + 3] = (*outer)[4 * i + 1] - border;

				DBG_PRINT_MSG("x: %f, y: %f -- x: %f, y: %f",
								(*outer)[4 * i + 0],
								(*outer)[4 * i + 1],
								(*outer)[4 * i + 2],
								(*outer)[4 * i + 3]);
			}

		}
	}

	void TabButton::GenerateBuffers (const Size& size, GLArrayBuffer* inner_buffer,
					GLArrayBuffer* outer_buffer)
	{
		std::vector<GLfloat> vertices;

		GenerateVertices(size, 1.f, 0, &vertices);

		if(outer_buffer) {
			if(!outer_buffer->IsBuffer()) {
				outer_buffer->Generate();
			}
			outer_buffer->Bind();
			outer_buffer->SetData(sizeof(GLfloat) * vertices.size(),
							&vertices[0]);
			outer_buffer->Reset();
		}

	}

}

