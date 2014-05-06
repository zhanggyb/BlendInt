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

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TextEntry.hpp>

#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Margin TextEntry::DefaultTextEntryPadding = Margin(2, 2, 2, 2);

	TextEntry::TextEntry ()
	: RoundWidget(),
	  m_vao(0),
	  m_start(0),
	  m_length(0),
	  m_cursor_position(0)
	{
		set_expand_x(true);
		set_size (120, 20);	// the same height of a button
		set_radius(0.0);

		InitializeTextEntry();
	}

	TextEntry::~TextEntry ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void TextEntry::Clear ()
	{
		m_text.clear();
		m_cursor_position = 0;
		m_start = 0;
		m_length = 0;
	}

	ResponseType TextEntry::KeyPressEvent (const KeyEvent& event)
	{
		if(!event.text().empty()) {
			m_text.insert(m_cursor_position, event.text());
			m_cursor_position += event.text().length();
			m_length += event.text().length();

			unsigned int text_width = m_font.GetTextWidth(m_text, m_length,
							m_start);
			unsigned int valid_width = size().width()
							- DefaultTextEntryPadding.left()
							- DefaultTextEntryPadding.right();

			if(text_width > valid_width) {
				m_start++;
				m_length--;

				text_width = m_font.GetTextWidth(m_text, m_length, m_start);
				while (text_width > valid_width) {
					m_start++;
					m_length--;
					text_width = m_font.GetTextWidth(m_text, m_length, m_start);
				}
			}

			Refresh();
			return Accept;

		} else {

			switch (event.key()) {

				case Key_Backspace: {
					DisposeBackspacePress();
					break;
				}

				case Key_Delete: {
					DisposeDeletePress();
					break;
				}

				case Key_Left: {
					DisposeLeftPress();
					break;
				}

				case Key_Right: {
					DisposeRightPress();
					break;
				}

				case Key_Up: {
					break;
				}

				case Key_Down: {
					break;
				}

				default:
					break;
			}

			return Accept;
		}
	}

	ResponseType TextEntry::MousePressEvent(const MouseEvent& event)
	{
		if(m_text.size()) {
			m_cursor_position = GetCursorPosition(event);
			Refresh();
		}

		return Accept;
	}

	void TextEntry::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormRoundRadius: {
					const float* radius_p = static_cast<const float*>(request.data());
					m_origin.set_x(*radius_p + DefaultTextEntryPadding.left());

					break;
				}

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					const Color& color = Theme::instance->text().inner;
					short shadetop = Theme::instance->text().shadetop;
					short shadedown = Theme::instance->text().shadedown;

					GenerateShadedFormBuffers(*size_p,
							round_type(),
							radius(),
							color,
							shadetop,
							shadedown,
							Vertical,
							m_inner_buffer.get(),
							m_outer_buffer.get());

					m_cursor_buffer->Bind();
					GLfloat* buf_p = (GLfloat*)m_cursor_buffer->Map(GL_READ_WRITE);
					*(buf_p + 5) = static_cast<float>(size_p->height()
									- DefaultTextEntryPadding.top()
									- DefaultTextEntryPadding.bottom());
					*(buf_p + 7) = static_cast<float>(size_p->height()
									- DefaultTextEntryPadding.top()
									- DefaultTextEntryPadding.bottom());
					m_cursor_buffer->Unmap();
					m_cursor_buffer->Reset();

					Refresh();
					break;
				}

				default:
					RoundWidget::Update(request);
			}

		}
	}

	ResponseType TextEntry::Draw (const RedrawEvent& event)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		DrawShadedTriangleFan(0, 1, m_inner_buffer.get());

		glDisableVertexAttribArray(1);

		glm::vec4 color;
		color.r = Theme::instance->text().outline.r() / 255.f;
		color.g = Theme::instance->text().outline.g() / 255.f;
		color.b = Theme::instance->text().outline.b() / 255.f;
		color.a = Theme::instance->text().outline.a() / 255.f;
		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("AA", 1);

		DrawTriangleStrip(0, m_outer_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		m_font.Print(mvp, m_origin.x(), m_origin.y(), m_text, m_length, m_start);

		if(focused()) {			// draw a cursor
			unsigned int cursor_pos = m_font.GetTextWidth(m_text,
						        m_cursor_position - m_start, m_start);
			glm::vec3 trans(cursor_pos + 1, 1, 0);
			glm::mat4 text_mvp = glm::translate(mvp, trans);

			glBindVertexArray(m_vao);

			//program = ShaderManager::instance->default_line_program();	// Now switch to line program
			program->Use();

			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(text_mvp));
			program->SetUniform1i("AA", 0);
			program->SetVertexAttrib4f("Color",	0.f, 55 / 255.f, 1.f, 175 / 255.f);

			glEnableVertexAttribArray(0);

			m_cursor_buffer->Bind();

			glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			m_cursor_buffer->Reset();

			glDisableVertexAttribArray(0);

			program->Reset();

			glBindVertexArray(0);
		}

		return Accept;
	}

	ResponseType TextEntry::FocusEvent (bool focus)
	{
		Refresh();
		return Accept;
	}

	void TextEntry::SetText (const String& text)
	{
		bool cal_width = true;

		m_text = text;

		m_text_outline = m_font.get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				Resize(size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				Resize(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = GetValidTextSize();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?
		m_origin.set_x(2);

		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		//SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void TextEntry::SetFont (const Font& font)
	{
		m_font = font;

		m_text_outline = m_font.get_text_outline(m_text);

		//m_length = GetVisibleTextLengthInCursorMove(m_text, m_start);

		m_origin.set_x(DefaultTextEntryPadding.left());
		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));
	}

	void TextEntry::InitializeTextEntry ()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_cursor_buffer.reset(new GLArrayBuffer);

		const Color& color = Theme::instance->text().inner;
		short shadetop = Theme::instance->text().shadetop;
		short shadedown = Theme::instance->text().shadedown;

		GenerateShadedFormBuffers(size(),
				round_type(),
				radius(),
				color,
				shadetop,
				shadedown,
				Vertical,
				m_inner_buffer.get(),
				m_outer_buffer.get()
				);

		std::vector<GLfloat> cursor_vertices(8);

		cursor_vertices[0] = 1.f;
		cursor_vertices[1] = static_cast<float>(DefaultTextEntryPadding.bottom());

		cursor_vertices[2] = 3.f;
		cursor_vertices[3] = static_cast<float>(DefaultTextEntryPadding.bottom());

		cursor_vertices[4] = 1.f;
		cursor_vertices[5] = static_cast<float>(size().height() - DefaultTextEntryPadding.top() - DefaultTextEntryPadding.bottom());

		cursor_vertices[6] = 3.f;
		cursor_vertices[7] = static_cast<float>(size().height() - DefaultTextEntryPadding.top() - DefaultTextEntryPadding.bottom());

		m_cursor_buffer->Generate();
		m_cursor_buffer->Bind();
		m_cursor_buffer->SetData(8 * sizeof(GLfloat), &cursor_vertices[0]);
		m_cursor_buffer->Reset();

		glBindVertexArray(0);

		m_origin.set_x(DefaultTextEntryPadding.left());
		m_origin.set_y((size().height() - m_font.get_height()) / 2
		                + std::abs(m_font.get_descender()));

		// set_preferred_size(m_text_outline.width(), m_text_outline.height());
		// set where start display the cursor
		//m_origin.set_x(m_origin.x() + static_cast<int>(radius()));
	}

	size_t TextEntry::GetValidTextSize ()
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
	
	void TextEntry::DisposeBackspacePress ()
	{
		if (m_text.size() && m_cursor_position > 0) {

			size_t valid_width = size().width()
							- DefaultTextEntryPadding.left()
							- DefaultTextEntryPadding.right();

			m_text.erase(m_cursor_position - 1, 1);
			m_cursor_position--;

			size_t text_width = 0;
			size_t len = m_text.length();
			while(len > 0)
			{
				text_width = m_font.GetReversedTextWidth(m_text, len, 0);
				if(text_width < valid_width) {
					break;
				}
				len--;
			}

			m_length = len;
			m_start = m_text.length() - m_length;

			Refresh();
		}
	}
	
	void TextEntry::DisposeDeletePress ()
	{
		if (m_text.size() && (m_cursor_position < m_text.length())) {

			size_t valid_width = size().width()
							- DefaultTextEntryPadding.left()
							- DefaultTextEntryPadding.right();

			m_text.erase(m_cursor_position, 1);

			size_t text_width = 0;

			size_t len = 0;
			while(len < (m_text.length() - m_start))
			{
				text_width = m_font.GetTextWidth(m_text, len, m_start);
				if(text_width > valid_width) {
					len--;
					break;
				}
				len++;
			}

			m_length = len;

			Refresh();
		}
	}
	
	void TextEntry::DisposeLeftPress ()
	{
		if (m_text.size() && m_cursor_position > 0) {

			size_t valid_width = size().width()
								- DefaultTextEntryPadding.left()
								- DefaultTextEntryPadding.right();

			m_cursor_position--;

			if (m_cursor_position < m_start) {
				m_start = m_cursor_position;

				size_t text_width = m_font.GetTextWidth(m_text, m_length,
								m_start);

				if(text_width < valid_width && m_length < (m_text.length() - m_start)) {
					m_length++;
					text_width = m_font.GetTextWidth(m_text, m_length, m_start);
					while(text_width < valid_width && m_length < (m_text.length() - m_start)) {
						m_length++;
						text_width = m_font.GetTextWidth(m_text, m_length, m_start);
					}
				}

				if(text_width > valid_width && m_length > 0) {
					m_length--;
					text_width = m_font.GetTextWidth(m_text, m_length, m_start);
					while ((text_width > valid_width) && (m_length > 0)) {
						m_length--;
						text_width = m_font.GetTextWidth(m_text, m_length, m_start);
					}
				}

			}

			Refresh();
		}

	}
	
	void TextEntry::DisposeRightPress ()
	{
		if (m_text.size() && m_cursor_position < m_text.length()) {
			m_cursor_position++;

			if (m_cursor_position > (m_start + m_length))
				m_start++;

			//m_length = GetVisibleTextLength(m_text, m_start);

			//DBG_PRINT_MSG("length: %lu, start: %lu, cursor: %lu",
			//				m_length, m_start, m_cursor_position);

			Refresh();
		}
	}

	void TextEntry::GetVisibleTextPlace (size_t* start, size_t* length)
	{
		size_t str_len = m_text.length();
		size_t width = m_font.GetTextWidth(m_text, str_len);

		if(width < (size().width() - 4)) {
			*start = 0;
			*length = str_len;
		} else {
			while(str_len > 0) {
				str_len--;
				width = m_font.GetTextWidth(m_text, str_len);
				if(width < (size().width() - 4)) break;
			}

			*start = m_text.length() - str_len;
			*length = str_len;
		}
	}
	
	int TextEntry::GetCursorPosition (const MouseEvent& event)
	{
		int text_width = m_font.GetTextWidth(m_text, m_length, m_start);
		int click_width = event.position().x() - position().x()
						- DefaultTextEntryPadding.left();

		if(click_width < 0 ||
		   click_width > static_cast<int>(size().width() - DefaultTextEntryPadding.right())) {
			return m_cursor_position;
		}

		int cursor_offset = 1;

		text_width = m_font.GetTextWidth(m_text, cursor_offset, m_start);
		if(text_width > click_width) {
			cursor_offset--;
		} else {
			cursor_offset++;
			text_width = m_font.GetTextWidth(m_text, cursor_offset, m_start);
			while((text_width < click_width) &&
				  (cursor_offset <= static_cast<int>(m_length))) {
				cursor_offset++;
				text_width = m_font.GetTextWidth(m_text, cursor_offset, m_start);
			}
		}

		return m_start + cursor_offset;
	}

}
