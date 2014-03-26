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
#include <BlendInt/Gui/Icon.hpp>

namespace BlendInt {

	/**
	 * @brief Items used in Menu, List, Toolbox
	 */
	class ActionItem: public Object
	{
	public:

		static RefPtr<ActionItem> Create ();

		static RefPtr<ActionItem> Create (const RefPtr<Icon>& icon, const std::string& text);

		static RefPtr<ActionItem> Create (const std::string& text);

		ActionItem ();

		ActionItem (const RefPtr<Icon>& icon, const std::string& text);

		ActionItem (const std::string& text);

		virtual ~ActionItem ();

		const RefPtr<Icon>& icon () const
		{
			return m_icon;
		}
		
		void set_icon (const RefPtr<Icon>& icon)
		{
			m_icon = icon;
		}
		
		const std::string& text () const
		{
			return m_text;
		}
		
		void set_text (const std::string& text)
		{
			m_text = text;
		}

	private:

		RefPtr<Icon> m_icon;

		std::string m_text;

		std::list<RefPtr<ActionItem> > m_list;
	};

}

#endif /* _BLENDINT_GUI_ACTIONITEM_HPP_ */
