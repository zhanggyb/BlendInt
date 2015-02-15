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

#include <iostream>

#include <boost/filesystem.hpp>

#include <gui/text.hpp>
#include <gui/filesystem-model.hpp>

namespace BlendInt {

	FileSystemModel::FileSystemModel()
	: AbstractItemModel(),
	  rows_(0),
	  columns_(DefaultColumns),	// temporary value
	  root_(0)
	{
		root_ = new ModelNode;
		RefPtr<Text> data(new Text("Root Node"));
		root_->data = data;
	}

	FileSystemModel::~FileSystemModel()
	{
		Clear();
		delete root_;
	}

	bool FileSystemModel::Load (const std::string& pathname)
	{
		namespace fs = boost::filesystem;
		bool is_path = false;

		fs::path path = fs::path(pathname);

		fs::file_status status;
		char buf[32];

		try {
			if (fs::exists(path) && fs::is_directory(path)) {

				Clear();
				assert(root_->child == 0);

				ModelNode* first = 0;
				ModelNode* tmp = 0;

				int i = 0, j = 0;
				fs::directory_iterator it(path);
				fs::directory_iterator it_end;
				while (it != it_end) {

					j = 0;

					status = fs::status(it->path());

					if(first == 0) {
						first = new ModelNode;
						first->parent = root_;
						root_->child = first;
						assert(first->up == 0);
					} else {
						first->down = new ModelNode;
						first->down->up = first;
						first = first->down;
					}

					assert(first->left == 0);

					first->data = RefPtr<Text>(new Text(it->path().filename().native()));

					j++;

					tmp = first;

					std::time_t time = fs::last_write_time(it->path());
					std::string time_str = std::asctime(std::localtime(&time));
					time_str.erase(time_str.size() - 1, 1);	// remove the '\n' char
					tmp->right = new ModelNode;
					tmp->right->data = RefPtr<Text>(new Text(time_str));
					tmp->right->left = tmp->right;
					tmp = tmp->right;
					j++;

					tmp->right = new ModelNode;
					tmp->right->data = RefPtr<Text>(new Text(fs::is_directory(it->path())? "d" : "-"));
					tmp->right->left = tmp->right;
					tmp = tmp->right;
					j++;

					if(fs::is_regular(it->path())) {
						snprintf(buf, 32, "%ld", fs::file_size(it->path()));
					} else {
						snprintf(buf, 32, " ");
					}
					tmp->right = new ModelNode;
					tmp->right->data = RefPtr<Text>(new Text(buf));
					tmp->right->left = tmp->right;
					tmp = tmp->right;
					j++;

					snprintf(buf, 32, "%o", status.permissions());
					tmp->right = new ModelNode;
					tmp->right->data = RefPtr<Text>(new Text(buf));
					tmp->right->left = tmp->right;
					j++;

					it++;
					i++;
				}

//				DBG_PRINT_MSG("ROWS: %d", i);
				assert(j == DefaultColumns);

				rows_ = i;
				columns_ = DefaultColumns;

				is_path = true;
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		return is_path;
	}

	void FileSystemModel::Clear ()
	{
		if(root_->child) {
			ModelNode* node = root_->child;
			ModelNode* tmp = 0;

			while(node) {
				tmp = node->down;
				DestroyRow(node);
				node = tmp;
			}

			root_->child = 0;
			rows_ = 0;
			columns_ = DefaultColumns;
		}
	}

	int FileSystemModel::GetRowCount (const ModelIndex& parent) const
	{
		if(root_->child) {
			return rows_;
		} else {
			return 0;
		}
	}

	int FileSystemModel::GetColumnCount (const ModelIndex& parent) const
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
		if (!parent.valid())
			return false;

		ModelNode* node = get_index_node(parent);
		if(node->child == 0) return false;

		assert(node == root_);
		node = node->child;

		assert(node->up == 0);

		ModelNode* next_view = 0;
		while(node) {
			next_view = node->down;
			InsertColumns (column, count, node);
			node = next_view;
		}

		columns_ += count;
		return true;
	}

	bool FileSystemModel::RemoveColumns (int column, int count,
	        const ModelIndex& parent)
	{
		if (!parent.valid())
			return false;

		ModelNode* node = get_index_node(parent);
		if (node->child == 0)
			return false;

		assert(node == root_);
		node = node->child;

		assert(node->up == 0);

		ModelNode* next_view = 0;
		while (node) {
			next_view = node->down;
			DestroyColumnsInRow(column, count, node);
			node = next_view;
		}

		if((column == 0) && (count == columns_)) {
			// remove all data
			rows_ = 0;
			columns_ = 0;
			return true;
		}

		int max = column + count;
		if(max > (columns_ - 1)) {
			count = count + columns_ - max;
		}

		columns_ -= count;

		return true;
	}

	bool FileSystemModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		if(!parent.valid()) return false;

		assert(count > 0);
		assert(row >= 0);

		if(columns_ == 0) {
			columns_ = DefaultColumns;
		}

		ModelNode* node = get_index_node(parent);

		// create count nodes
		ModelNode* first = 0;
		ModelNode* last = 0;
		ModelNode* tmp = 0;

		char buf[32];
		first = new ModelNode;
		first->data = RefPtr<Text>(new Text("row 0, col 0"));

		// add (columns - 1) nodes at right
		tmp = first;
		for(int j = 1; j < columns_; j++) {
			tmp->right = new ModelNode;
			snprintf(buf, 32, "row 0, col %d", j);
			tmp->right->data = RefPtr<Text>(new Text(buf));
			tmp->right->left = tmp;
			tmp = tmp->right;
		}

		last = first;

		for(int i = 1; i < count; i++) {
			last->down = new ModelNode;
			snprintf(buf, 32, "row %d, col 0", i);
			last->down->data = RefPtr<Text>(new Text(buf));
			last->down->up = last;

			// add (columns - 1) nodes at right
			tmp = last->down;
			for(int j = 1; j < columns_; j++) {
				tmp->right = new ModelNode;
				snprintf(buf, 32, "row %d, col %d", i, j);
				tmp->right->data = RefPtr<Text>(new Text(buf));
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

		rows_ += count;
		return true;
	}

	bool FileSystemModel::RemoveRows (int row, int count,
			const ModelIndex& parent)
	{
		if (!parent.valid())
			return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = get_index_node(parent);
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

			int i = 0;
			for(; i < count; i++) {
				last = node->down;
				DestroyRow(node);
				node = last;
				if(node == 0) {
					i++;
					break;
				}
			}

			node = get_index_node(parent);
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

			rows_ -= i;
			return true;
		}

		return false;
	}

	ModelIndex FileSystemModel::GetRootIndex () const
	{
		ModelIndex index;
		set_index_node(index, root_);

		return index;
	}

	ModelIndex FileSystemModel::GetIndex (int row, int column,
			const ModelIndex& parent) const
	{
		ModelIndex index;
		if(!parent.valid()) return index;

		ModelNode* parent_node = get_index_node(parent);
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
				set_index_node(index, node);
			}

		}

		return index;
	}

#ifdef DEBUG

	void FileSystemModel::Print ()
	{
		DBG_PRINT_MSG("%s", "Print all data: ");
		ModelNode* node = root()->child;

		int i = 0;
		while(node) {
			PrintRow(node);
			node = node->down;
			i++;
		}

		//assert(i == rows_);
		DBG_PRINT_MSG("print rows: %d", i);
		DBG_PRINT_MSG("rows recorded: %d", rows_);
	}

	void FileSystemModel::PrintRow(ModelNode* first)
	{
		assert(first);
		assert(first->left == 0);

		if(first->child) {
			PrintRow(first->child);
		}

		ModelNode* tmp = first;

		int i = 0;
		while (tmp) {
			if(i == 0) {
				assert(tmp->left == 0);
			} else {
				assert(tmp->up == 0);
				assert(tmp->down == 0);
			}

			//std::cout << ConvertFromString(tmp->data).c_str() << " ";
			tmp = tmp->right;

			i++;
		}
		std::cout << std::endl;
	}

#endif	// DEBUG

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

	void FileSystemModel::InsertColumns (int column, int count, ModelNode* node)
	{
		assert(node);
		assert(node->left == 0);

		// create count nodes
		ModelNode* first = 0;
		ModelNode* last = 0;
		ModelNode* tmp = 0;

		char buf[32];
		first = new ModelNode;
		first->data = RefPtr<Text>(new Text("new col 0"));

		// add (columns - 1) nodes at right
		tmp = first;
		for(int j = 1; j < count; j++) {
			tmp->right = new ModelNode;
			snprintf(buf, 32, "new col %d", j);
			tmp->right->data = RefPtr<Text>(new Text(buf));
			tmp->right->left = tmp;
			tmp = tmp->right;
		}
		last = tmp;

		tmp = node;
		while(tmp->right && (column > 0)) {
			tmp = tmp->right;
			column--;
		}

		if(column == 0) {	// Insert
			if(tmp->left == 0) {	// Insert 0
				if(tmp->parent) {
					tmp->parent->child = first;
					first->parent = tmp->parent;
					tmp->parent = 0;
				}

				first->up = tmp->up;
				if(tmp->up) {
					tmp->up->down = first;
				}
				first->down = tmp->down;
				if(tmp->down) {
					tmp->down->up = first;
				}

				last->right = tmp;
				tmp->left = last;

				tmp->up = 0;
				tmp->down = 0;

				assert(first->left == 0);
			} else {

				tmp->left->right = first;
				first->left = tmp->left;
				last->right = tmp;
				tmp->left = last;

			}
		} else {	// too large row given, append to tail
			tmp->right = first;
			first->left = tmp;
		}
	}

	void FileSystemModel::DestroyColumnsInRow (int column, int count,
	        ModelNode* node)
	{
		assert(node);
		assert(node->left == 0);

		if(node->child) {
			DestroyColumnsInRow(column, count, node->child);
			node->child = 0;
		}

		while(node->right && (column > 0)) {
			node = node->right;
			column--;
		}

		if(column == 0) {

			ModelNode* up = node->up;
			ModelNode* down = node->down;
			ModelNode* parent = node->parent;
			ModelNode* tmp = 0;

			if (node->left == 0) {	// the first column in this model

				if (parent) {	// debug
					assert(up == 0);
				}

				for (int i = 0; i < count; i++) {
					tmp = node->right;
					delete node;
					node = tmp;

					if (node == 0)
						break;
				}

				if (node) {
					node->up = up;
					node->down = down;
					node->left = 0;

					if (up)
						up->down = node;

					if (down)
						down->up = node;

					node->parent = parent;	// mostly is 0
					if(parent) {
						parent->child = node;
					}

				} else {	// remove the whole row

					if (up)
						up->down = down;

					if (down) {
						down->parent = parent;
						down->up = up;
					}

					if(parent) {
						parent->child = down;
					}

				}

			} else {

				ModelNode* first = node->left;
				ModelNode* tmp = 0;

				for (int i = 0; i < count; i++) {
					tmp = node->right;
					delete node;
					node = tmp;

					if (node == 0)
						break;
				}

				first->right = node;

				if(node) {	// remove columns in the middle
					node->left = first;
				}
			}
		}
	}

	void FileSystemModel::DestroyColumn (ModelNode* node)
	{

	}

}

