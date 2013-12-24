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

#include <list>

#include <BlendInt/String.hpp>

#ifndef _BLENDINT_MENUITEM_HPP_
#define _BLENDINT_MENUITEM_HPP_

namespace BlendInt {

	class MenuItem
	{
	public:

		MenuItem ();

		MenuItem (const String& text);

		~MenuItem();

		void set_highlight (bool highlight)
		{
			m_highlight = highlight;
		}

		void PushBack (MenuItem* item);

		void PushFront (MenuItem* item);

		void Append (MenuItem* subitem);

		void LinkTo (MenuItem* parentitem);

	private:

		MenuItem* m_up;
		MenuItem* m_prev;
		MenuItem* m_next;

		std::list<MenuItem*> m_subs;
		bool m_highlight;

		String m_text;
	};

}

#endif /* _BLENDINT_MENUITEM_HPP_ */
