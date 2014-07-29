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

#include <BlendInt/Gui/FileSystemModel.hpp>

namespace BlendInt {

	FileSystemModel::FileSystemModel()
	: AbstractItemModel(),
	  rows_(0),
	  columns_(3),
	  root_(0)
	{
		root_ = new ModelNode;
		root_->data = String("Root Node");
	}

	FileSystemModel::~FileSystemModel()
	{
		ClearAllChildNodes ();

		delete root_;
	}

	bool FileSystemModel::Load (const std::string& pathname)
	{
		namespace fs = boost::filesystem;
		bool is_path = false;

		path_ = fs::path(pathname);
		fs::file_status status;
		//char buf[32];

		try {
			if (fs::exists(path_)) {

				if (fs::is_directory(path_)) {

					fs::directory_iterator it(path_);
					fs::directory_iterator it_end;
					while (it != it_end) {

						status = fs::status(it->path());

						it++;
					}

					is_path = true;
				}
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		return is_path;
	}

	int FileSystemModel::GetRows (const ModelIndex& parent) const
	{
		if(root_->child) {
			return rows_;
		} else {
			return 0;
		}
	}

	int FileSystemModel::GetColumns (const ModelIndex& parent) const
	{
		if(root_->child) {
			return columns_;
		} else {
			return 0;
		}
	}

	bool FileSystemModel::InsertColumns (int column, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	bool FileSystemModel::RemoveColumns (int column, int count,
			const ModelIndex& parent)
	{
		return false;
	}

	bool FileSystemModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		if(!parent.IsValid()) return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = GetIndexNode(parent);

		// create count nodes
		ModelNode* first = 0;
		ModelNode* last = 0;
		ModelNode* tmp = 0;

		char buf[32];
		first = new ModelNode;
		first->data = String("row 0, col 0");

		// add (columns - 1) nodes at right
		tmp = first;
		for(int j = 1; j < columns_; j++) {
			tmp->right = new ModelNode;
			snprintf(buf, 32, "row 0, col %d", j);
			tmp->right->data = String(buf);
			tmp->right->left = tmp;
			tmp = tmp->right;
		}

		last = first;

		for(int i = 1; i < count; i++) {
			last->down = new ModelNode;
			snprintf(buf, 32, "row %d, col 0", i);
			last->down->data = String(buf);
			last->down->up = last;

			// add (columns - 1) nodes at right
			tmp = last->down;
			for(int j = 1; j < columns_; j++) {
				tmp->right = new ModelNode;
				snprintf(buf, 32, "row %d, col %d", i, j);
				tmp->right->data = String(buf);
				tmp->right->left = tmp;
				tmp = tmp->right;
			}

			last = last->down;
		}

		// if the node has no child, create and append count rows
		if(node->child == 0) {
			node->child = first;
			first->parent = node;
		} else {
			node = node->child;
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

	bool FileSystemModel::RemoveRows (int row, int count,
			const ModelIndex& parent)
	{
		if (!parent.IsValid())
			return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = GetIndexNode(parent);
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

			node = GetIndexNode(parent);
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

	ModelIndex FileSystemModel::GetRootIndex () const
	{
		ModelIndex index;
		SetIndexNode(index, root_);

		return index;
	}

	ModelIndex FileSystemModel::GetIndex (int row, int column,
			const ModelIndex& parent) const
	{
		ModelIndex index;
		if(!parent.IsValid()) return index;

		ModelNode* parent_node = GetIndexNode(parent);
		ModelNode* node = parent_node->child;

		if(node == 0) return index;

		// move row down
		while(node->down && (row > 0)) {
			node = node->down;
			row--;
		}

		if(row == 0) {

			while(node->right && (column > 0)) {
				node = node->right;
				column--;
			}

			if(column == 0) {
				SetIndexNode(index, node);
			}

		}

		return index;
	}

#ifdef DEBUG

	void FileSystemModel::Print ()
	{
		ModelNode* node = root()->child;

		int i = 0;
		while(node) {
			PrintRow(node);
			node = node->down;
			i++;
		}
	}

	void FileSystemModel::PrintRow(ModelNode* first)
	{
		assert(first);
		assert(first->left == 0);

		if(first->child) {
			PrintRow(first->child);
		}

		ModelNode* tmp = first;
		while (tmp) {
			DBG_PRINT_MSG("%s", ConvertFromString(tmp->data).c_str());
			tmp = tmp->right;
		}
	}

#endif	// DEBUG

	void FileSystemModel::ClearAllChildNodes()
	{
		ModelNode* node = root_->child;

		if(node) {

			ModelNode* next = 0;
			while (node) {
				next = node->down;
				DestroyRow(node);
				node = next;
			}

			root_->child = 0;
		}
	}

	void FileSystemModel::DestroyRow (ModelNode* node)
	{
		assert(node);
		assert(node->left == 0);

		if (node->child) {
			DestroyRow(node->child);
			node->child = 0;
		}

		ModelNode* right = 0;
		while (node) {
			right = node->right;
			delete node;
			node = right;
		}
	}

	void FileSystemModel::DestroyColumn (ModelNode* node)
	{

	}

}

