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
#include <algorithm>
#include <iterator>

#include <BlendInt/Gui/MenuItemBin.hpp>

namespace BlendInt {

	MenuItemBin::MenuItemBin()
	: m_parent (0)
	{
	}

	MenuItemBin::MenuItemBin (const String& title)
	: m_title(title), m_parent (0)
	{
	}

	MenuItemBin::~MenuItemBin ()
	{
#ifdef DEBUG
		std::wcout << "Delete MenuBin: " << m_title << std::endl;
#endif
		std::list<MenuItem*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			(*it)->m_parent = 0;
			delete *it;
		}
		m_list.clear();

		if(m_parent)
			m_parent->m_sub = 0;
	}

	MenuItem* MenuItemBin::GetMenuItem (size_t index)
	{
		std::list<MenuItem*>::iterator it = m_list.begin();
		std::advance(it, index);

		if(it == m_list.end()) return 0;

		return *it;
	}

	void MenuItemBin::Add (const String& text)
	{
		MenuItem* new_item = new MenuItem (text);

		new_item->m_parent = this;
		m_list.push_back(new_item);
	}

	void MenuItemBin::Add (Icon* icon, const String& text)
	{
		MenuItem* new_item = new MenuItem (icon, text);

		new_item->m_parent = this;
		m_list.push_back(new_item);
	}

	void MenuItemBin::Add (Icon* icon, const String& text, const String& shortcut)
	{
		MenuItem* new_item = new MenuItem (icon, text, shortcut);

		new_item->m_parent = this;
		m_list.push_back(new_item);
	}

	void MenuItemBin::Add (MenuItem* item)
	{
		if(!item) return;
		if(item->m_parent == this) return;

		m_list.push_back(item);
		item->m_parent = this;
	}

	void MenuItemBin::SetParent (MenuItem* item)
	{
		if(m_parent == item) return;

		if(m_parent) {
			m_parent->m_sub = 0;
		}

		if(item)
			item->m_sub = this;

		m_parent = item;
	}

	void MenuItemBin::Remove (MenuItem *item)
	{
		if(!item) return;

		std::list<MenuItem*>::iterator it = std::find (m_list.begin(), m_list.end(), item);

		if(it == m_list.end()) return;

		m_list.erase(it);
		item->m_parent = 0;
	}

	void MenuItemBin::Delete (MenuItem* item)
	{
		if(!item) return;

		std::list<MenuItem*>::iterator it = std::find (m_list.begin(), m_list.end(), item);

		if(it == m_list.end()) return;

		m_list.erase(it);
		item->m_parent = 0;

		delete item;
	}

#ifdef DEBUG
	void MenuItemBin::print_menu_items()
	{
		std::list<MenuItem*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			std::wcout << "MenuBin Item Text: " << (*it)->text() << std::endl;
			if((*it)->m_sub) {
				(*it)->m_sub->print_menu_items();
			}
		}
	}
#endif

}

