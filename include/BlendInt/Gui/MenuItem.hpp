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

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/AbstractIcon.hpp>

#ifndef _BLENDINT_MENUITEM_HPP_
#define _BLENDINT_MENUITEM_HPP_

namespace BlendInt {

	class MenuItemBin;

	class MenuItem
	{
	public:

		friend class MenuItemBin;

		MenuItem ();

		MenuItem (const String& text);

		MenuItem (const String& text, const String& shortcut);

		MenuItem (AbstractIcon* icon, const String& text);

		MenuItem (AbstractIcon* icon, const String& text, const String& shortcut);

		~MenuItem();

		void SetIcon (AbstractIcon* icon);

		inline void set_text (const String& text)
		{
			m_text = text;
		}

		inline const String& text () const {return m_text;}

		RefPtr<AbstractIcon> icon() const {return m_icon;}

		inline void set_shortcut (const String& shortcut)
		{
			m_shortcut = shortcut;
		}

		inline const String& shortcut () const {return m_shortcut;}

		inline void set_highlight (bool highlight)
		{
			m_highlight = highlight;
		}

		void SetParentMenu (MenuItemBin* parent);

		void SetSubMenu (MenuItemBin* sub);

	private:

		void RemoveIcon ();

		RefPtr<AbstractIcon> m_icon;

		String m_text;
		String m_shortcut;

		MenuItemBin* m_parent;
		MenuItemBin* m_sub;

		bool m_highlight;
	};

}

#endif /* _BLENDINT_MENUITEM_HPP_ */
