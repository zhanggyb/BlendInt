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
	: m_status_down(false), m_checkable(false),
	  m_status_checked(false), m_status_hover(false)
	{
		FontCache::create(m_font);
	}

	AbstractButton::AbstractButton (AbstractWidget *parent)
		: Frame(parent), m_status_down(false), m_checkable(false),
		  m_status_checked(false), m_status_hover(false)
	{
		FontCache::create(m_font);
	}

	AbstractButton::~AbstractButton ()
	{

	}

	void AbstractButton::set_text (const String& text)
	{
		if(text.empty()) {
			resize (90, 25);
			return;
		}

		m_text = text;

		/* Box in which hold the text */
		Rect text_outline;
		text_outline = FontCache::create(m_font)->get_text_outline(m_text);
		resize (text_outline.width() + padding().left() + padding().right(), text_outline.height() + padding().top() + padding().bottom());
	}

	void AbstractButton::set_font (const Font& font)
	{
		m_font = font;
		FontCache::create(m_font);

		/* Box in which hold the text */
		Rect text_outline;
		text_outline = FontCache::create(m_font)->get_text_outline(m_text);

		resize (text_outline.width() + padding().left() + padding().right(),
				text_outline.height() + padding().top() + padding().bottom());
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

} /* namespace BlendInt */
