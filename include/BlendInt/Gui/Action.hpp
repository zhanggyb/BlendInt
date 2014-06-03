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

#ifndef _BLENDINT_GUI_ACTIONITEM_HPP_
#define _BLENDINT_GUI_ACTIONITEM_HPP_

#include <list>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Margin.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/Icon.hpp>

namespace BlendInt {

	/**
	 * @brief Items used in Menu, List, Toolbox
	 */
	class Action: public Object
	{
	public:

		static RefPtr<Action> Create ();

		static RefPtr<Action> Create (const String& text);

		static RefPtr<Action> Create (const String& text, const String& shortcut);

		static RefPtr<Action> Create (const RefPtr<Icon>& icon, const String& text);

		static RefPtr<Action> Create (const RefPtr<Icon>& icon, const String& text, const String& shortcut);

		Action ();

		explicit Action (const String& text);

		Action (const String& text, const String& shortcut);

		Action (const RefPtr<Icon>& icon, const String& text);

		Action (const RefPtr<Icon>& icon, const String& text, const String& shortcut);

		virtual ~Action ();

		const RefPtr<Icon>& icon () const
		{
			return m_icon;
		}
		
		/**
		 * @brief Add one sub item
		 * @param[in] item A sub item
		 * @param[in] check If need to check whether the item already exist
		 */
		void AddSubItem (const RefPtr<Action>& item, bool check = false);

		/**
		 * @brief Get size if icon and text alighed horizontally
		 */
		Size GetHSize (const Font& font, const Margin& margin, int space);

		unsigned int GetTextLength (const Font& font);

		void set_icon (const RefPtr<Icon>& icon)
		{
			m_icon = icon;
		}
		
		const String& text () const
		{
			return m_text;
		}
		
		void set_text (const String& text)
		{
			m_text = text;
		}

		const String& shortcut () const
		{
			return m_shortcut;
		}

		void set_shortcut (const String& shortcut)
		{
			m_shortcut = shortcut;
		}

		const std::list<RefPtr<Action> >& list () const
		{
			return m_list;
		}

	private:

		RefPtr<Icon> m_icon;

		String m_text;

		String m_shortcut;

		std::list<RefPtr<Action> > m_list;
	};

}

#endif /* _BLENDINT_GUI_ACTIONITEM_HPP_ */
