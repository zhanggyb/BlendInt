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

#include <GL/glew.h>

#include <BlendInt/AbstractButton.hpp>

#include <iostream>

namespace BlendInt {

	AbstractButton::AbstractButton ()
	: RoundWidget(),
			m_status_down(false), m_checkable(false),
	  m_status_checked(false), m_status_hover(false), m_length(0)
	{
		FontCache::create(m_font);
	}

	AbstractButton::AbstractButton (AbstractWidget *parent)
		: RoundWidget(parent), m_status_down(false), m_checkable(false),
		  m_status_checked(false), m_status_hover(false), m_length(0)
	{
		FontCache::create(m_font);
	}

	AbstractButton::~AbstractButton ()
	{

	}

	void AbstractButton::set_text (const String& text)
	{
		if(text.empty()) {
			Resize (90, 20);
			return;
		}

		bool cal_width = true;

		m_text = text;
		FontCache* fc = FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				dynamic_cast<AbstractExtraForm*>(this)->Resize(size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				dynamic_cast<AbstractExtraForm*>(this)->Resize(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = get_valid_text_size();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?
		m_origin.set_x((size().width() - m_text_outline.width()) / 2);

		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void AbstractButton::set_font (const Font& font)
	{
		bool cal_width = true;

		m_font = font;
		FontCache* fc =	FontCache::create(m_font);

		m_text_outline = fc->get_text_outline(m_text);

		m_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				dynamic_cast<AbstractExtraForm*>(this)->Resize(size().width(), m_text_outline.height());
			} else {
				m_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				dynamic_cast<AbstractExtraForm*>(this)->Resize(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_length = get_valid_text_size();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?

		m_origin.set_x((size().width() - m_text_outline.width()) / 2);

		m_origin.set_y((size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void AbstractButton::press_mouse (MouseEvent* event)
	{
		if (!contain(event->position())) return;

		if (m_checkable) {
			m_status_checked = !m_status_checked;
			m_toggled.fire(m_status_checked);

		} else {
			m_status_down = true;
			m_clicked.fire();
		}
		event->accept(this);
	}

	void AbstractButton::release_mouse(MouseEvent* event)
	{
		m_status_down = false;

		if (! contain(event->position()))	return;

		if (m_checkable) {

		} else {

		}
	}

	void AbstractButton::move_mouse (MouseEvent* event)
	{
		if (m_status_down) {
			event->accept(this);
			return;
		}

		if (contain(event->position())) {
			m_status_hover = true;
            event->accept(this);
		} else {
			m_status_hover = false;
			m_status_down = false;
		}
	}

	size_t AbstractButton::get_valid_text_size()
	{
		size_t width = 0;

		size_t str_len = m_text.length();

		width = FontCache::create(m_font)->get_text_width(m_text, str_len);

		if(width > size().width()) {
			while(str_len > 0) {
				width = FontCache::create(m_font)->get_text_width(m_text, str_len);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}

} /* namespace BlendInt */
