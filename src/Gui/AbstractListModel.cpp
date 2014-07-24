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

#include <BlendInt/Gui/AbstractListModel.hpp>

namespace BlendInt {

	AbstractListModel::AbstractListModel()
	: AbstractItemModel(),
	  m_root(0)
	{
		m_root = new ModelNode;
	}

	AbstractListModel::~AbstractListModel ()
	{
		ClearAllChildNodes();

		delete m_root;
	}

	int AbstractListModel::GetRows (const ModelIndex& parent) const
	{
		return 0;
	}

	int AbstractListModel::GetColumns (const ModelIndex& parent) const
	{
		return 0;
	}

	bool AbstractListModel::InsertColumns (int column, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	bool AbstractListModel::RemoveColumns (int column, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	bool AbstractListModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	bool AbstractListModel::RemoveRows (int row, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	ModelIndex AbstractListModel::GetRootIndex () const
	{
		ModelIndex retval;
		SetIndexNode(retval, m_root);

		return retval;
	}

	ModelIndex AbstractListModel::GetIndex (int row, int column,
			const ModelIndex& parent) const
	{
		ModelIndex retval;

		return retval;
	}

#ifdef DEBUG

	void AbstractListModel::Print ()
	{
	}

	void AbstractListModel::ClearAllChildNodes()
	{
		// TODO: delete all node data


	}

#endif

}
