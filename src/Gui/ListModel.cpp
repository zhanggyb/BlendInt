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

#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Gui/ListModel.hpp>

namespace BlendInt {

	ItemDeque::ItemDeque ()
	: m_parent(0),
	  m_child(0)
	{
	}

	ItemDeque::~ItemDeque ()
	{
		DBG_PRINT_MSG("%s", "delete this item deque");
	}

	ListModel::ListModel()
	: Object()
	{

	}

	ListModel::~ListModel()
	{
		Clear();
	}

	void ListModel::PushBack (ItemDeque* deque)
	{
		if(deque == 0) return;

		if(m_list.size() == 0) {
			m_list.push_back(deque);
		} else {

			size_t columns = m_list.front()->columns();

			if(deque->columns() != columns) {
				DBG_PRINT_MSG("Warning: %s", "the item columns does not match");

				deque->resize(columns);
			}

			m_list.push_back(deque);
		}
	}

	int ListModel::GetColumns () const
	{
		if(m_list.size() == 0) {
			return 0;
		} else {
			return m_list.front()->columns();
		}
	}

	int ListModel::GetRows () const
	{
		return m_list.size();
	}

	void ListModel::Clear ()
	{
		for(std::list<ItemDeque*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			delete *it;
		}

		m_list.clear();
	}

#ifdef DEBUG

	void ListModel::Print()
	{
		for(std::list<ItemDeque*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			for(std::deque<String>::const_iterator dec_it = (*it)->deque().begin(); dec_it != (*it)->deque().end(); dec_it++)
			{
				DBG_PRINT_MSG("%s ", ConvertFromString(*dec_it).c_str());
			}
			DBG_PRINT_MSG("%s", " -- ");
		}
	}

#endif

}

