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

#include <cassert>

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

#endif	// DEBUG

	void AbstractListModel::DestroyChildNode (ModelNode* node)
	{
		assert(node);
		assert(node->left == 0);	// only the first left node has child

		if(node->child) {
			DestroyChildNode(node->child);
			node->child = 0;
		}

		ModelNode* parent = node->parent;
		ModelNode* tmp = 0;

		while(node) {

			tmp = node->down;

			DestroyRow(node);
			node = tmp;
			node->parent = parent;	// no needed but looks resonable

		}
	}

	void AbstractListModel::DestroyRow (ModelNode* node)
	{
		assert(node);
		assert(node->child == 0);
		assert(node->left == 0);

		if(node->child) {
			DestroyChildNode(node->child);
			node->child = 0;
		}

		ModelNode* tmp = 0;
		while (node) {

			if(node->up) {
				node->up->down = node->down;
			}
			if(node->down) {
				node->down->up = node->up;
			}

			tmp = node->right;
			delete node;
			node = tmp;
		}
	}

	void AbstractListModel::DestroyColumn (ModelNode* node)
	{
		assert(node);
		assert(node->up == 0);

		// TODO: check the child node and delete the column if the child exists

		ModelNode* tmp = 0;
		while (node) {

			if(node->left) {
				node->left->right = node->right;
			}
			if(node->right) {
				node->right->left = node->left;
			}

			tmp = node->down;
			delete node;
			node = tmp;

		}
	}

	void AbstractListModel::ClearAllChildNodes()
	{
		// TODO: delete all node data

		ModelNode* node = m_root->child;

		ModelNode* next = 0;
		while(node){
			next = node->child;
			DestroyRow(node);
			node = next;
		}

	}

}
