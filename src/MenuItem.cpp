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

#include <BlendInt/MenuItem.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/FontCache.hpp>

#include <BlendInt/StockIcon.hpp>

namespace BlendInt {

	MenuItem::MenuItem ()
	: m_icon(0), m_parent(0), m_sub(0), m_highlight(false)
	{
	}

	MenuItem::MenuItem (const String& text)
	: m_icon(0), m_text(text), m_parent(0), m_sub(0), m_highlight(false)
	{
	}

	MenuItem::MenuItem (const String& text, const String& shortcut)
	: m_icon(0), m_text(text), m_shortcut(shortcut), m_parent(0), m_sub(0), m_highlight(false)
	{
	}


	MenuItem::MenuItem (Icon* icon, const String& text)
	: m_icon(icon), m_text(text), m_parent(0), m_sub(0), m_highlight(false)
	{
	}

	MenuItem::MenuItem (Icon* icon, const String& text, const String& shortcut)
	: m_icon(icon), m_text(text), m_shortcut(shortcut), m_parent(0), m_sub(0), m_highlight(false)
	{
	}

	MenuItem::~MenuItem()
	{
#ifdef DEBUG
		std::wcout << "Delete MenuItem: " << m_text << std::endl;
#endif

		if(m_sub) {
			m_sub->m_parent = 0;
			delete m_sub;
			m_sub = 0;
		}

		if(m_parent) {
			m_parent->Remove(this);
		}

		RemoveIcon();
	}

	void MenuItem::SetIcon(Icon* icon)
	{
		if(m_icon == icon) return;

		RemoveIcon();

		m_icon = icon;
	}

	void MenuItem::SetParentMenu(MenuItemBin* parent)
	{
		if(m_parent == parent) return;

		if(parent) {
			parent->m_list.push_back(this);
		}

		m_parent = parent;
	}

	void MenuItem::SetSubMenu (MenuItemBin* sub)
	{
		if(m_sub == sub) return;

		if(m_sub) {
			m_sub->m_parent = 0;
			// delete m_sub;
		}

		if(sub) {
			sub->SetParent(this);
		}
	}

	void MenuItem::RemoveIcon()
	{
		if(m_icon) {
			if(!m_icon->GetReferenceCount())
				delete m_icon;

			m_icon = 0;
		}
	}

}

