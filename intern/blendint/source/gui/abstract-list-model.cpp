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

#include <gui/abstract-list-model.hpp>
#include <gui/text.hpp>

namespace BlendInt {

	AbstractListModel::AbstractListModel()
	: AbstractItemModel(),
	  root_(0)
	{
		root_ = new ModelNode;
		root_->data = RefPtr<Text>(new Text("Root Node"));
	}

	AbstractListModel::~AbstractListModel ()
	{
		ClearAllChildNodes();

		delete root_;
	}

	bool AbstractListModel::InsertColumns (int column, int count,
			const ModelIndex& superview)
	{
		return false;
	}

	bool AbstractListModel::RemoveColumns (int column, int count,
			const ModelIndex& superview)
	{
		return false;
	}

	bool AbstractListModel::InsertRows (int row, int count,
			const ModelIndex& superview)
	{
		if(!superview.IsValid()) return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = get_index_node(superview);

		ModelNode* first = 0;
		ModelNode* last = 0;

		char buf[8];
		first = new ModelNode;
		first->data = RefPtr<Text>(new Text("row 0"));
		last = first;

		for(int i = 1; i < count; i++) {
			last->down = new ModelNode;
			snprintf(buf, 8, "row %d", i);
			last->down->data = RefPtr<Text>(new Text(buf));
			last->down->up = last;
			last = last->down;
		}

		// if the node has no child, create and append count rows
		if(node->child == 0) {
			node->child = first;
			first->parent = node;
		} else {
			node = node->child;
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
			const ModelIndex& superview)
	{
		if (!superview.IsValid())
			return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = get_index_node(superview);
		if(node->child == 0)
			return false;

		node = node->child;

		while(node->down && (row > 0)) {
			node = node->down;
			row--;
		}

		if(row == 0) {

			ModelNode* first = node->up;
			ModelNode* last = node;

			for(int i = 0; i < count; i++)
			{
				last = node->down;
				DestroyRow(node);
				node = last;

				if(node == 0)
					break;
			}

			node = get_index_node(superview);
			if(first == 0) {

				if(last == 0) {	// clear the list
					node->child = 0;
				} else {	// remove the first count rows from the original list
					node->child = last;
					last->parent = node;
					last->up = 0;
				}

			} else {
				first->down = last;
				if(last) {
					last->up = first;
				}
			}

			return true;
		}

		return false;
	}

	ModelIndex AbstractListModel::GetRootIndex () const
	{
		ModelIndex retval;
		set_index_node(retval, root_);

		return retval;
	}

	ModelIndex AbstractListModel::GetIndex (int row, int column,
			const ModelIndex& superview) const
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

		ModelNode* superview = node->parent;
		ModelNode* tmp = 0;

		while(node) {

			tmp = node->down;

			DestroyRow(node);
			node = tmp;
			node->parent = superview;	// no needed but looks resonable

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
		ModelNode* node = root_->child;

		if(node) {

			ModelNode* next_view = 0;
			while(node){
				next_view = node->down;
				DestroyRow(node);
				node = next_view;
			}

			root_->child = 0;

		}
	}

}
