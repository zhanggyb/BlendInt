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

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/FontCache.hpp>

#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Margin TextEntry::DefaultTextEntryPadding = Margin(2, 2, 2, 2);

	TextEntry::TextEntry ()
	: RoundWidget(), m_start(0), m_length(0), m_cursor_position(0), m_timer(0), m_flicker(true)
	{
		InitOnce();
	}

	TextEntry::~TextEntry ()
	{
	}

	void TextEntry::KeyPressEvent (KeyEvent* event)
	{
		if(event->text().size()) {

			m_text.insert(m_cursor_position, event->text());
			m_cursor_position += event->text().length();
			m_length += event->text().length();

			FontCache* fc = FontCache::create(m_font);
			unsigned text_width = fc->GetTextWidth(m_text, m_length, m_start);

			unsigned int valid_width = size().width() - DefaultTextEntryPadding.left() - DefaultTextEntryPadding.right();

			if(text_width > valid_width) {
				m_length -= event->text().length();
				m_start += event->text().length();
			}

			text_width = fc->GetTextWidth(m_text, m_length, m_start);
			while (text_width > valid_width) {
				m_length--;
				m_start++;
				text_width = fc->GetTextWidth(m_text, m_length, m_start);
			}

			Refresh();
			event->accept(this);
			return;
		}

		switch(event->key()) {

			case Key_Backspace: {
				break;
			}

			case Key_Delete: {
				break;
			}

			case Key_Left: {

				m_cursor_position--;
				if(m_cursor_position < 0)
					m_cursor_position = 0;

				if(m_cursor_position < static_cast<int>(m_start)) {
					m_start--;
				}

				AdjustVisibleTextLength();
				
				break;
			}

			case Key_Right: {

				m_cursor_position++;
				if(m_cursor_position > static_cast<int>(m_text.length()))
					m_cursor_position = m_text.length();

				if(m_cursor_position > static_cast<int>(m_start + m_length)) {
					m_start++;
				}

				AdjustVisibleTextLength();

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

		event->accept(this);
	}

	void TextEntry::MousePressEvent(MouseEvent* event)
	{
		std::cout << "pressed" << std::endl;

		if(m_text.size()) {
			//m_cursor_position = GetCursorPosition(event);
		}

		event->accept(this);
	}

	bool TextEntry::Update (int type, const void* data)
	{
		switch (type) {

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				m_origin.set_x(*radius_p + DefaultTextEntryPadding.left());

				return true;
			}

			case FormSize: {
				//const Size* size_p = static_cast<const Size*>(data);
				//GenerateFormBuffer(size_p, round_type(), radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);

				const Size* size_p = static_cast<const Size*>(data);
				const Color& color = themes()->text.inner;
				short shadetop = themes()->text.shadetop;
				short shadedown = themes()->text.shadedown;

				GenerateShadedFormBuffers(size_p,
						round_type(),
						radius(),
						color,
						shadetop,
						shadedown,
						Vertical,
						5,
						m_inner_buffer.get(),
						m_outer_buffer.get(),
						0
						);
				return true;
			}

			default:
				return RoundWidget::Update(type, data);
		}
	}

	void TextEntry::Draw (RedrawEvent* event)
	{
		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		GLint xy_attrib = program->GetAttributeLocation("xy");
		GLint color_attrib = program->GetAttributeLocation("color");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		glEnableVertexAttribArray(xy_attrib);
		glEnableVertexAttribArray(color_attrib);

		m_inner_buffer->Bind();

		glVertexAttribPointer(xy_attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(0)					 // offset of first element
							  );

		glVertexAttribPointer(color_attrib, // attribute
							  4,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(2 * sizeof(GLfloat))					 // offset of first element
							  );

		glDrawArrays(GL_POLYGON, 0, m_inner_buffer->GetBufferSize() / (6 * sizeof(GLfloat)));

		glDisableVertexAttribArray(color_attrib);

		m_inner_buffer->Reset();

		GLfloat outline_color[4] = {themes()->text.outline.r() / 255.f,
				themes()->text.outline.g() / 255.f,
				themes()->text.outline.b() / 255.f,
				(themes()->text.outline.a() / WIDGET_AA_JITTER) / 255.f
		};

		program->SetVertexAttrib4fv("color", outline_color);

		glm::vec3 jitter;
		glm::mat4 jitter_matrix;

		m_outer_buffer->Bind();

		glVertexAttribPointer(xy_attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));

			glDrawArrays(GL_QUAD_STRIP, 0, m_outer_buffer->GetBufferSize() / (2 * sizeof(GLfloat)));
		}

		m_outer_buffer->Reset();

		glDisableVertexAttribArray(xy_attrib);

		program->Reset();

		FontCache* fc = FontCache::create(m_font);

		fc->Print(mvp, m_origin.x(), m_origin.y(), m_text, m_length, m_start);

		// draw a cursor
		/*
		unsigned int text_width = fc->GetTextWidth(m_text, m_cursor_position - m_start, m_start);

		if(focused() && m_flicker) {
			glTranslatef(text_width + 1, 2, 0);
			glColor4ub(0, 125, 255, 175);
			glRecti(0, 0, DefaultTextEntryPadding.bottom(), size().height() - DefaultTextEntryPadding.top() - DefaultTextEntryPadding.bottom());
		}
		*/

		event->accept(this);
	}

	void TextEntry::SetText (const String& text)
	{
		bool cal_width = true;

		m_text = text;
		FontCache* fc = FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

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
		m_origin.set_x(2);

		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void TextEntry::SetFont (const Font& font)
	{
		m_font = font;
		FontCache* fc =	FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

		AdjustVisibleTextLength();

		m_origin.set_x(DefaultTextEntryPadding.left());
		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));
	}

	void TextEntry::InitOnce ()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		set_expand_x(true);
		set_size (120, 24);	// the same height of a button
		set_radius(0.0);

		FontCache* fc = FontCache::create(m_font);

		const Color& color = themes()->text.inner;
		short shadetop = themes()->text.shadetop;
		short shadedown = themes()->text.shadedown;

		GenerateShadedFormBuffers(&size(),
				round_type(),
				radius(),
				color,
				shadetop,
				shadedown,
				Vertical,
				5,
				m_inner_buffer.get(),
				m_outer_buffer.get(),
				0
				);

		m_origin.set_x(DefaultTextEntryPadding.left());
		m_origin.set_y((size().height() - fc->get_height()) / 2
		                + std::abs(fc->get_descender()));

		// set_preferred_size(m_text_outline.width(), m_text_outline.height());
		set_preferred_size(size());
		set_minimal_size(
		        DefaultTextEntryPadding.left()
		                + DefaultTextEntryPadding.right(),
		        DefaultTextEntryPadding.top()
		                + DefaultTextEntryPadding.bottom());

		// set where start display the cursor
		m_origin.set_x(m_origin.x() + static_cast<int>(radius()));

		// and set timer
		m_timer.reset(new Timer);
		m_timer->SetInterval(500);	// 100 ms
		events()->connect(m_timer->timeout(), this, &TextEntry::OnReverseCursor);

		m_timer->Start();
	}

	void TextEntry::OnReverseCursor()
	{
		m_flicker = m_flicker ? false: true;
	}

	size_t TextEntry::GetValidTextSize ()
	{
		size_t width = 0;
		FontCache* fc = FontCache::create(m_font);

		size_t str_len = m_text.length();

		width = fc->GetTextWidth(m_text, str_len);

		if(width > size().width()) {
			while(str_len > 0) {
				width = fc->GetTextWidth(m_text, str_len);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}

	void TextEntry::GetVisibleTextPlace (size_t* start, size_t* length)
	{
		size_t str_len = m_text.length();
		FontCache* fc = FontCache::create(m_font);

		size_t width = fc->GetTextWidth(m_text, str_len);

		if(width < size().width() - 4) {
			*start = 0;
			*length = str_len;
		} else {
			while(str_len > 0) {
				str_len--;
				width = fc->GetTextWidth(m_text, str_len);
				if(width < size().width() - 4) break;
			}

			*start = m_text.length() - str_len;
			*length = str_len;
		}

	}

	void TextEntry::AdjustVisibleTextLength ()
	{
		FontCache* fc = FontCache::create(m_font);
		unsigned text_width = fc->GetTextWidth(m_text, m_length, m_start);
		unsigned int valid_width = size().width() - DefaultTextEntryPadding.left() - DefaultTextEntryPadding.right();

		if(text_width > valid_width) {
			m_length--;
			text_width = fc->GetTextWidth(m_text, m_length, m_start);
			while ((text_width > valid_width) && (m_length > 0)) {
				m_length--;
				text_width = fc->GetTextWidth(m_text, m_length, m_start);
			}
		}
	}


	int TextEntry::GetCursorPosition (const MouseEvent* event)
	{
		FontCache* fc = FontCache::create(m_font);
		int text_width = fc->GetTextWidth(m_text, m_length, m_start);
		int click_width = event->position().x() - position().x() - DefaultTextEntryPadding.left();

		if(click_width < 0 ||
		   click_width > static_cast<int>(size().width() - DefaultTextEntryPadding.right())) {
			return m_cursor_position;
		}

		int cursor_offset = 1;

		text_width = fc->GetTextWidth(m_text, cursor_offset, m_start);
		if(text_width > click_width) {
			cursor_offset--;
		} else {
			cursor_offset++;
			text_width = fc->GetTextWidth(m_text, cursor_offset, m_start);
			while((text_width < click_width) &&
				  (cursor_offset <= static_cast<int>(m_length))) {
				cursor_offset++;
				text_width = fc->GetTextWidth(m_text, cursor_offset, m_start);
			}
		}

		return m_start + cursor_offset;
	}

}
