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

#include <BlendInt/TextEntry.hpp>
#include <BlendInt/FontCache.hpp>

#include <iostream>

namespace BlendInt {

	TextEntry::TextEntry ()
	: RoundWidget(), m_start(0), m_length(0), m_cursor_position(0), m_timer(0), m_flicker(true)
	{
		Init();
	}

	TextEntry::TextEntry (AbstractWidget* parent)
	: RoundWidget(parent), m_start(0), m_length(0), m_cursor_position(0), m_timer(0), m_flicker(true)
	{
		Init();
	}

	TextEntry::~TextEntry ()
	{
	}

	void TextEntry::Draw ()
	{
		RoundWidget::Draw();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_BLEND);

		FontCache::create(m_font)->Print(m_origin.x(), m_origin.y(), m_text, m_length, m_start, false);

		if(m_flicker) {
			//glTranslatef(2, 2, 0);
			glColor4ub(0, 255, 255, 200);
			glRecti(0, -m_origin.y() + 2, 2, 14);
		}

		glPopMatrix();

	}

	void TextEntry::KeyPressEvent (KeyEvent* event)
	{
		if(event->key() == Key_Backspace) {
			if(m_text.size())
				m_text.erase(m_text.length() - 1, 1);
		} else {
			m_text += event->text();
		}

		//m_length = GetValidTextSize();
		GetVisibleTextPlace(&m_start, &m_length);
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
		m_origin.set_x(4);

		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void TextEntry::SetFont (const Font& font)
	{
		bool cal_width = true;

		m_font = font;
		FontCache* fc =	FontCache::create(m_font);

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

		m_origin.set_x(4);

		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void TextEntry::Init ()
	{
		bool cal_width = true;

		set_expand_x(true);
		set_size (100, 24);	// the same height of a button

		FontCache* fc = FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

		m_length = m_text.length();

		if (size().height() < m_text_outline.height()) {
			if (expand_y()) {
				set_size(size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if (size().width() < m_text_outline.width()) {
			if (expand_x()) {
				set_size(m_text_outline.width(), size().height());
			} else {
				if (cal_width)
					m_length = GetValidTextSize();
			}
		}

		m_origin.set_x(4);
		m_origin.set_y(
		        (size().height() - fc->get_height()) / 2
		                + std::abs(fc->get_descender()));

		// set_preferred_size(m_text_outline.width(), m_text_outline.height());
		set_preferred_size(size());

		// and set timer
		m_timer = new Timer(this);
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

		width = fc->get_text_width(m_text, str_len);

		if(width > size().width()) {
			while(str_len > 0) {
				width = fc->get_text_width(m_text, str_len);
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

		size_t width = fc->get_text_width(m_text, str_len);

		if(width < size().width() - 4) {
			*start = 0;
			*length = str_len;
		} else {
			while(str_len > 0) {
				str_len--;
				width = fc->get_text_width(m_text, str_len);
				if(width < size().width() - 4) break;
			}

			*start = m_text.length() - str_len;
			*length = str_len;
		}

	}

}

