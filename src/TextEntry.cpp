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

	Margin TextEntry::DefaultTextEntryPadding = Margin(2, 2, 2, 2);

	TextEntry::TextEntry ()
	: RoundWidget(), m_start(0), m_length(0), m_cursor_position(0), m_timer(0), m_flicker(true)
	{
		InitOnce();
	}

	TextEntry::TextEntry (AbstractWidget* parent)
	: RoundWidget(parent), m_start(0), m_length(0), m_cursor_position(0), m_timer(0), m_flicker(true)
	{
		InitOnce();
	}

	TextEntry::~TextEntry ()
	{
	}

	void TextEntry::Update (int type, const void* data)
	{
		switch (type) {

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				m_origin.set_x(*radius_p + DefaultTextEntryPadding.left());
				m_cursor_position += static_cast<int>(*radius_p - radius());
				break;
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
				return;
			}

			default:
				break;
		}
		RoundWidget::Update(type, data);
	}

	void TextEntry::Draw ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ThemeManager* tm = ThemeManager::instance();

		/*
		glColor4ub(tm->themes()->text.inner_sel.r(),
					tm->themes()->text.inner_sel.g(),
					tm->themes()->text.inner_sel.b(),
					tm->themes()->text.inner_sel.a());
		*/

		DrawShadedInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->text.outline.r(),
		        themes()->text.outline.g(),
		        themes()->text.outline.b(),
		        themes()->text.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		FontCache::create(m_font)->Print(m_origin.x(), m_origin.y(), m_text, m_length, m_start);

		if(focused() && m_flicker) {
			glTranslatef(m_cursor_position, 2, 0);
			glColor4ub(0, 125, 255, 175);
			glRecti(0, 0, 2, 18);
		}

		glDisable(GL_BLEND);

		glPopMatrix();

	}

	void TextEntry::KeyPressEvent (KeyEvent* event)
	{
		if(event->key() == Key_Backspace) {
			if(m_text.size()) {
				m_text.erase(m_text.length() - 1, 1);
			}
		} else {
			m_text += event->text();
		}

		//m_length = GetValidTextSize();
		GetVisibleTextPlace(&m_start, &m_length);
		m_cursor_position = FontCache::create(m_font)->GetTextWidth(m_text, m_text.length());
		m_cursor_position += static_cast<int>(radius()) + DefaultTextEntryPadding.left() + 1;
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

	void TextEntry::InitOnce ()
	{
		m_inner_buffer.reset(new GLArrayBufferF);
		m_outer_buffer.reset(new GLArrayBufferF);

		bool cal_width = true;

		set_expand_x(true);
		set_size (100, 24);	// the same height of a button
		set_radius(0.0);

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

		m_origin.set_x(2);
		m_origin.set_y(
		        (size().height() - fc->get_height()) / 2
		                + std::abs(fc->get_descender()));

		// set_preferred_size(m_text_outline.width(), m_text_outline.height());
		set_preferred_size(size());

		// set where start display the cursor
		m_cursor_position = static_cast<int>(radius()) + DefaultTextEntryPadding.left();

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

}
