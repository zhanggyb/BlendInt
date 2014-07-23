/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_LISTMODEL_HPP_
#define _BLENDINT_GUI_LISTMODEL_HPP_

#include <deque>
#include <list>
#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/String.hpp>

namespace BlendInt {

	/**
	 * @brief Item deque used as a row in list view, tree view
	 */
	class ItemDeque
	{
	public:

		ItemDeque ();

		virtual ~ItemDeque ();

		void push_back (const String& val)
		{
			m_deque.push_back(val);
		}

		void push_front (const String& val)
		{
			m_deque.push_front(val);
		}

		size_t columns () const
		{
			return m_deque.size();
		}

		void resize (size_t n)
		{
			m_deque.resize(n, String(""));
		}

		const std::deque<String>& deque () const
		{
			return m_deque;
		}

	private:

		ItemDeque* m_parent;

		ItemDeque* m_child;

		std::deque<String> m_deque;	// temporarily, use String
	};

	class ListModel: public Object
	{
	public:

		ListModel ();

		virtual ~ListModel ();

		void PushBack (ItemDeque* deque);

		int GetColumns () const;

		int GetRows () const;

		void Clear ();

#ifdef DEBUG

		void Print ();

#endif

	protected:

		std::list<ItemDeque*>& list ()
		{
			return m_list;
		}

	private:

		std::list<ItemDeque*> m_list;
	};

}

#endif /* _BLENDINT_GUI_LISTMODEL_HPP_ */
