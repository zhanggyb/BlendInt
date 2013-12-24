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

#include <BlendInt/MenuItem.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/FontCache.hpp>

namespace BlendInt {

	MenuItem::MenuItem ()
	: m_up(0), m_prev(0), m_next(0), m_highlight(false)
	{
	}

	MenuItem::MenuItem (const String& text)
	: m_up(0), m_prev(0), m_next(0), m_highlight(false), m_text(text)
	{
	}


	MenuItem::~MenuItem()
	{
		if(m_up) {
			m_up->m_subs.remove(this);
			m_up = 0;
		}

		if(m_prev) {
			m_prev->m_next = m_next;
		}

		if(m_next) {
			m_next->m_prev = m_prev;
		}

		std::list<MenuItem*>::iterator it;
		for(it = m_subs.begin(); it != m_subs.end(); it++)
		{
			(*it)->m_up = 0;
			delete *it;
		}
		m_subs.clear();
	}

	void MenuItem::PushBack (MenuItem* item)
	{
		if(!item) return;
		if(item == m_next) return;

		if(m_next)
			delete m_next;

		m_next = item;
		item->m_prev = this;

		// TODO: set item's parent
	}

	void MenuItem::PushFront (MenuItem* item)
	{
		if(!item) return;
		if(item == m_prev) return;

		if (m_prev)
			delete m_prev;

		m_prev = item;
		item->m_next = this;

		// TODO: set item's parent
	}

	void MenuItem::Append (MenuItem* subitem)
	{
	}

	void MenuItem::LinkTo (MenuItem* parentitem)
	{
	}

}

