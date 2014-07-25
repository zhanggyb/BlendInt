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

#include <BlendInt/Gui/StringListModel.hpp>

namespace BlendInt {

	StringListModel::StringListModel()
	: AbstractListModel(),
	  m_rows(0)
	{

	}

	StringListModel::~StringListModel()
	{

	}

	int StringListModel::GetRows (const ModelIndex& parent) const
	{
		return m_rows;
		/*
		ModelNode* node = root()->child;

		int count = 0;
		while(node) {
			count++;
			node = node->down;
		}

		return count;
		*/
	}

	int StringListModel::GetColumns (const ModelIndex& parent) const
	{
		return 1;
	}

	bool StringListModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		bool retval = false;

		retval = AbstractListModel::InsertRows(row, count, parent);
		if (retval) {
			m_rows += count;
		}

		return retval;
	}

#ifdef DEBUG

	void StringListModel::Print()
	{
		ModelNode* node = root()->child;

		int i = 0;
		while(node) {
			DBG_PRINT_MSG("node %d: %s", i, ConvertFromString(node->data).c_str());
			node = node->down;
			i++;
		}
	}

#endif

}
