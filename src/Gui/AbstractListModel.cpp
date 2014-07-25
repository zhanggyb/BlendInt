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
		m_root->data = String("Root Node");
	}

	AbstractListModel::~AbstractListModel ()
	{
		ClearAllChildNodes();

		delete m_root;
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
		if(!parent.IsValid()) return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = GetIndexNode(parent);

		// if the node has no child, create and append count rows
		if(node->child == 0) {

			ModelNode* first = 0;
			ModelNode* last = 0;

			char buf[8];
			first = new ModelNode;
			first->data = String("row 0");
			last = first;

			for(int i = 1; i < count; i++) {
				last->down = new ModelNode;
				snprintf(buf, 8, "row %d", i);
				last->down->data = String(buf);
				last->down->up = last;
				last = last->down;
			}

			node->child = first;
			first->parent = node;

		} else {

			node = node->child;
			ModelNode* first = 0;
			ModelNode* last = 0;

			char buf[8];
			first = new ModelNode;
			first->data = String("-- 0");
			last = first;

			for(int i = 1; i < count; i++) {
				last->down = new ModelNode;
				snprintf(buf, 8, "-- %d", i);
				last->down->data = String(buf);
				last->down->up = last;
				last = last->down;
			}

			// find wher to insert the new list
			while(node->down && (row > 0)) {
				node = node->down;
				row--;
			}

			if(row == 0) {	// Insert

				if(node->up == 0) {	// Insert 0

					node->parent->child = first;
					first->parent = node->parent;
					node->parent = 0;

					last->down = node;
					node->up = last;

				} else {

					node->up->down = first;
					first->up = node->up;

					last->down = node;
					node->up = last;

				}

			} else {	// too large row given, append to tail

				node->down = first;
				first->up = node;
			}


		}

		return true;
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
		ModelNode* node = m_root->child;

		if(node) {

			ModelNode* next = 0;
			while(node){
				next = node->down;
				DestroyRow(node);
				node = next;
			}

		}
	}

}
