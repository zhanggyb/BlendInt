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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Label::Label (const String& text)
		: Widget(),
		  m_text(text),
		  m_length(0),
		  m_alignment(AlignLeft),
		  m_background_color(0x00000000),
		  m_vao(0)
	{
		set_expand_x(true);

		set_size(0, 24);	// the same height of a Buttons

		InitOnce();
	}

	Label::~Label ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Label::SetText (const String& text)
	{
		bool cal_width = true;

		m_text = text;

		m_text_outline = m_font.get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				Resize(this, size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				Resize(this, m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = GetValidTextSize();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?
		if(m_alignment & AlignLeft) {
			m_origin.set_x(0);
		} else if(m_alignment & AlignRight) {
			m_origin.set_x(size().width() - m_text_outline.width());
		} else if(m_alignment & AlignVerticalCenter) {
			m_origin.set_x((size().width() - m_text_outline.width()) / 2);
		}

		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void Label::SetFont (const Font& font)
	{
		bool cal_width = true;

		m_font = font;

		m_text_outline = m_font.get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				Resize(this, size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				Resize(this, m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = GetValidTextSize();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?

		if(m_alignment & AlignLeft) {
			m_origin.set_x(0);
		} else if(m_alignment & AlignRight) {
			m_origin.set_x(size().width() - m_text_outline.width());
		} else if(m_alignment & AlignVerticalCenter) {
			m_origin.set_x((size().width() - m_text_outline.width()) / 2);
		}

		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	bool Label::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());

					if(size_p->height() < m_text_outline.height()) {
						m_length = 0;
					} else {
						m_origin.set_y((size_p->height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));
						m_length = GetValidTextSize(size_p);
					}

					if(size_p->width() < m_text_outline.width()) {
						m_origin.set_x(0);
					} else {
						if(m_alignment & AlignLeft) {
							m_origin.set_x(0);
						} else if(m_alignment & AlignRight) {
							m_origin.set_x(size_p->width() - m_text_outline.width());
						} else if(m_alignment & AlignVerticalCenter) {
							m_origin.set_x((size_p->width() - m_text_outline.width()) / 2);
						}
					}

					glBindVertexArray(m_vao);

					m_rect->Bind();

					std::vector<GLfloat> vertices(12);
					GenerateFlatRectVertices(*size_p, 0.f, &vertices);

					m_rect->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);

					m_rect->Reset();

					glBindVertexArray(0);

					return true;
				}

				case FormPreferredSize: {
					return true;
				}

				default:
					return Widget::Update(request);
			}

		} else {
			return false;
		}
	}

	ResponseType Label::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color",
						m_background_color.r() / 255.f,
						m_background_color.g() / 255.f,
						m_background_color.b() / 255.f,
						m_background_color.a() / 255.f);
		program->SetUniform1i("AA", 0);

		m_rect->Bind();
		glEnableVertexAttribArray(0);	// 0 is the locaiton in shader

		glVertexAttribPointer(
						0, // attribute
						2,		// number of elements per vertex, here (x,y)
						GL_FLOAT,	// the type of each element
						GL_FALSE,	// take our values as-is
						0,		// no extra data between each position
						BUFFER_OFFSET(0)	// the first element
		);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		glDisableVertexAttribArray(0);

		m_rect->Reset();

		program->Reset();
		glBindVertexArray(0);

		m_font.Print(mvp, m_origin.x(), m_origin.y(), m_text, m_length, 0);

		return Accept;
	}

	size_t Label::GetValidTextSize()
	{
		size_t width = 0;

		size_t str_len = m_text.length();

		width = m_font.GetTextWidth(m_text, str_len);

		if(width > size().width()) {
			while(str_len > 0) {
				width = m_font.GetTextWidth(m_text, str_len);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}

	size_t Label::GetValidTextSize(const Size* size)
	{
		size_t width = 0;

		size_t str_len = m_text.length();

		width = m_font.GetTextWidth(m_text, str_len);

		if(width > size->width()) {
			while(str_len > 0) {
				width = m_font.GetTextWidth(m_text, str_len);
				if(width < size->width()) break;
				str_len--;
			}
		}

		return str_len;
	}

	void Label::InitOnce ()
	{
		bool cal_width = true;

		m_text_outline = m_font.get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				set_size(size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				set_size(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = GetValidTextSize();
			}
		}

		m_origin.set_x(0);
		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		set_preferred_size(size());

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_rect.reset(new GLArrayBuffer);
		m_rect->Generate();
		m_rect->Bind();
		std::vector<GLfloat> vertices(12);
		GenerateFlatRectVertices(size(), 0.f, &vertices);
		m_rect->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_rect->Reset();
		glBindVertexArray(0);
	}

} /* namespace BlendInt */

