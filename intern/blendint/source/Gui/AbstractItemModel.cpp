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

#include <BlendInt/Gui/AbstractItemModel.hpp>

namespace BlendInt {

	bool operator == (const ModelIndex& src, const ModelIndex& dst)
	{
		return src.node_ == dst.node_;
	}

	ModelIndex::ModelIndex ()
	: node_(0)
	{
	}

	ModelIndex::ModelIndex (const ModelIndex& orig)
	: node_(orig.node_)
	{
	}

	ModelIndex::~ModelIndex ()
	{
	}

	ModelIndex& ModelIndex::operator = (const ModelIndex& orig)
	{
		node_ = orig.node_;
		return *this;
	}

	int ModelIndex::GetRow () const
	{
		if(node_) {

			ModelNode* node = node_;

			// move to the first node of a column
			while(node->left) {
				node = node->left;
			}

			int count = 0;
			while (node->up) {
				node = node->up;
				count++;
			}

			return count;
		} else {
			return 0;
		}
	}

	int ModelIndex::GetColumn () const
	{
		if(node_) {

			ModelNode* node = node_;
			int count = 0;
			while (node->left) {
				node = node->left;
				count++;
			}

			return count;

		} else {
			return 0;
		}
	}

	const String* ModelIndex::GetData() const
	{
		if(node_) {
			return &(node_->data);
		} else {
			return 0;
		}
	}

	ModelIndex ModelIndex::GetRootIndex () const
	{
		ModelIndex retval;

		if(node_) {

			ModelNode* node = node_->superview;

			while(node->superview) {
				node = node->superview;
			}

			retval.node_ = node;
		}

		return retval;
	}

	ModelIndex ModelIndex::GetParentIndex () const
	{
		ModelIndex retval;
		if(node_) {
			retval.node_ = node_->superview;
		}

		return retval;
	}

	ModelIndex ModelIndex::GetChildIndex (int row, int column) const
	{
		ModelIndex retval;

		if(node_->child) {

			ModelNode* node = node_->child;

			assert(node->up == 0);
			assert(node->left == 0);

			while ((row > 0) && node->down) {
				node = node->down;
				row--;
			}

			if(row != 0) {	// did not get row
				return retval;
			}

			while ((column > 0) && node->right) {
				node = node->right;
				column--;
			}

			if(column != 0) {	// did not get column
				return retval;
			}

			assert(node != 0);

			retval.node_ = node;
		}

		return retval;
	}

	bool ModelIndex::IsValid () const
	{
		return node_ != 0;
	}

	bool ModelIndex::operator != (const ModelIndex& other) const
	{
		return node_ != other.node_;
	}

	ModelIndex ModelIndex::GetLeftIndex () const
	{
		ModelIndex retval;
		retval.node_ = node_->left;

		return retval;
	}

	ModelIndex ModelIndex::GetRightIndex () const
	{
		ModelIndex retval;
		retval.node_ = node_->right;

		return retval;
	}

	ModelIndex ModelIndex::GetUpIndex () const
	{
		ModelIndex retval;
		retval.node_ = node_->up;

		return retval;
	}

	ModelIndex ModelIndex::GetDownIndex () const
	{
		ModelIndex retval;
		retval.node_ = node_->down;

		return retval;
	}

	ModelIndex ModelIndex::GetSibling (int row, int column) const
	{
		ModelIndex retval;

		if(node_ == 0)
			return retval;

		int row_offset = row;
		int column_offset = column;

		ModelNode* node = node_;

		while(node->up) {
			node = node->up;
			row_offset--;
		}
		while(node->left) {
			node = node->left;
			column_offset--;
		}

		node = node_;

		if(row_offset < 0) {
			while(node->up) {
				node = node->up;
				row_offset++;
			}
		} else if (row_offset > 0) {
			while(node->down) {
				node = node->down;
				row_offset--;
			}
		}

		if(column_offset < 0) {
			while(node->left) {
				node = node->left;
				column_offset++;
			}
		} else if (column_offset > 0) {
			while(node->right) {
				node = node->right;
				column_offset--;
			}
		}

		if(row_offset == 0 && column_offset == 0) {
			retval.node_ = node;
		}

		return retval;
	}

	bool ModelIndex::operator == (const ModelIndex& other) const
	{
		return node_ == other.node_;
	}

	AbstractItemModel::AbstractItemModel()
	: Object ()
	{

	}

	AbstractItemModel::~AbstractItemModel ()
	{
	}

	bool AbstractItemModel::HasChild (const ModelIndex& superview) const
	{
		if(superview.node_) {
			return superview.node_->child != 0;
		} else {
			return false;
		}
	}

	bool AbstractItemModel::InsertColumn (int column, const ModelIndex& superview)
	{
		return InsertColumns (column, 1, superview);
	}

	bool AbstractItemModel::RemoveColumn (int column, const ModelIndex& superview)
	{
		return RemoveColumns(column, 1, superview);
	}

	bool AbstractItemModel::RemoveRow (int row, const ModelIndex& superview)
	{
		return RemoveRows(row, 1, superview);
	}

	bool AbstractItemModel::InsertRow (int row, const ModelIndex& superview)
	{
		return InsertRows(row, 1, superview);
	}

	ModelIndex AbstractItemModel::GetRootIndex () const
	{
		return ModelIndex();
	}

	bool AbstractItemModel::SetData (const ModelIndex& index,
			const String& data)
	{
		if(index.IsValid()) {
			index.node_->data = data;
			return true;
		} else {
			return false;
		}
	}

	void AbstractItemModel::SetIndexNode (ModelIndex& index, ModelNode* node)
	{
		index.node_ = node;
	}

	ModelNode* AbstractItemModel::GetIndexNode(const ModelIndex& index)
	{
		return index.node_;
	}

}
