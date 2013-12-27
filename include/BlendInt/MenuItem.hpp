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

#include <BlendInt/Menu.hpp>
#include <BlendInt/String.hpp>
#include <BlendInt/FormBase.hpp>

#ifndef _BLENDINT_MENUITEM_HPP_
#define _BLENDINT_MENUITEM_HPP_

namespace BlendInt {

	class Menu;

	class MenuItem
	{
	public:

		friend class Menu;

		MenuItem ();

		MenuItem (const String& text);

		MenuItem (const String& text, const String& shortcut);

		MenuItem (FormBase* icon, const String& text);

		MenuItem (FormBase* icon, const String& text, const String& shortcut);

		~MenuItem();

		void SetIcon (FormBase* icon);

		inline void set_text (const String& text)
		{
			m_text = text;
		}

		inline const String& text () const {return m_text;}

		inline void set_shortcut (const String& shortcut)
		{
			m_shortcut = shortcut;
		}

		inline const String& shortcut () const {return m_shortcut;}

		inline void set_highlight (bool highlight)
		{
			m_highlight = highlight;
		}

		void SetParentMenu (Menu* parent);

		void SetSubMenu (Menu* sub);

	private:

		void RemoveIcon ();

		FormBase* m_icon;

		String m_text;
		String m_shortcut;

		Menu* m_parent;
		Menu* m_sub;

		bool m_highlight;
	};

}

#endif /* _BLENDINT_MENUITEM_HPP_ */
