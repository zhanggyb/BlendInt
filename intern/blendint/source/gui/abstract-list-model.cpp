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
	}

	AbstractListModel::~AbstractListModel ()
	{
		ClearAllChildNodes();

		delete root_;
	}

	bool AbstractListModel::InsertColumns (int column, int count,
	        const ModelIndex& parent)
	{
		if(!parent.valid()) return false;

		assert(count > 0);
		assert(column >= 0);

		ModelNode* node = get_index_node(parent);

		ModelNode* first = 0;
		ModelNode* last = 0;

		if(node->child == 0) {	// if the node has no child, create and append 1 row with count columns

			first = new ModelNode;
			last = first;
			for(int i = 1; i < count; i++) {
				last->right = new ModelNode;
				last->right->left = last;
				last = last->right;
			}
			node->child = first;
			first->parent = node;

		} else {

			node = node->child;
			ModelNode* next = 0;

			while(node) {

				next = node->down;

				ModelNode* tmp = node;

				// find where to insert the new columns
				while(tmp->right && (column > 0)) {
					tmp = tmp->right;
					column--;
				}

				first = new ModelNode;
				last = first;
				for(int i = 1; i < count; i++) {
					last->right = new ModelNode;
					last->right->left = last;
					last = last->right;
				}

				if(column == 0) {	// insert

					if(tmp->left == 0) {	// insert 0

						if(tmp->parent) {
							tmp->parent->child = first;
							first->parent = tmp->parent;
							tmp->parent = 0;
						}

						if(tmp->up) {
							tmp->up->down = first;
							first->up = tmp->up;
							tmp->up = 0;
						}

						if(tmp->down) {
							tmp->down->up = first;
							first->down = tmp->down;
							tmp->down = 0;
						}

						last->right = tmp;
						tmp->left = last;

					} else {

						assert(tmp->up == 0);
						assert(tmp->down == 0);

						tmp->left->right = first;
						first->left = tmp->left;
						last->right = tmp;
						tmp->left = last;

					}

				} else {	// append

					tmp->right = first;
					first->left = tmp;

				}

				node = next;
			}

		}

		return true;
	}

	bool AbstractListModel::RemoveColumns (int column, int count,
	        const ModelIndex& parent)
	{
		if(!parent.valid()) return false;

		assert(count > 0);
		assert(column >= 0);

		ModelNode* node = get_index_node(parent);
		if(node->child == 0) return false;

		node = node->child;

		ModelNode* up_record = 0;
		ModelNode* down_record = 0;

		while(node) {

			ModelNode* tmp = node;	// the tmp node iterate in this row
			up_record = node->up;
			down_record = node->down;

			while (tmp->right && (column > 0)) {
				tmp = tmp->right;
				column--;
			}

			if(column == 0) {

				ModelNode* first = tmp->left;
				ModelNode* last = tmp;

				for(int i = 0; i < count; i++) {
					last = tmp->right;

					// destroy tmp:

					if(tmp->up) {
						tmp->up->down = 0;
					}
					if(tmp->left) {
						tmp->left->right = 0;
					}
					if(tmp->right) {
						tmp->right->left = 0;
					}
					if(tmp->down) {
						tmp->down->up = 0;
					}

					delete tmp;

					tmp = last;
					if(tmp == 0) break;
				}

				tmp = get_index_node(parent);

				if(up_record == 0) {	// the first child

					if(first == 0) {

						if(last == 0) {
							tmp->child = 0;
						} else {
							tmp->child = last;
							last->parent = tmp;
							last->left = 0;
							last->up = 0;
						}

						up_record = last;

					} else {

						first->right = last;
						if(last) {
							last->left = first;
						}

						up_record = first;
					}

				} else {

					if(first == 0) {

						if(last == 0) {
							// never reach this
						} else {
							up_record->down = last;
							last->up = up_record;
							last->left = 0;
						}

						up_record = last;

					} else {

						first->right = last;
						if(last) {
							last->left = first;
						}
						up_record->down = first;
						first->up = up_record;

						up_record = first;

					}

				}

			} else {	// no column in this model
				return false;
			}

			node = down_record;
		}

		return true;
	}

	bool AbstractListModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		if(!parent.valid()) return false;

		assert(count > 0);
		assert(row >= 0);

		ModelNode* node = get_index_node(parent);

		ModelNode* first = 0;
		ModelNode* last = 0;

		first = new ModelNode;
		last = first;

		for(int i = 1; i < count; i++) {
			last->down = new ModelNode;
			last->down->up = last;
			last = last->down;
		}

		if(node->child == 0) {	// if the node has no child, create and append 1 column with count rows
			node->child = first;
			first->parent = node;
		} else {
			node = node->child;
			// find where to insert the new rows
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

                    ModelNode* row_iter = first;
                    ModelNode* ref_iter = node;
                    ModelNode* tmp1 = 0;
                    ModelNode* tmp2 = 0;
                    
                    while(row_iter) {
                        
                        tmp1 = row_iter;
                        
                        while(ref_iter->right) {   // add one row
                            
                            tmp2 = new ModelNode;
                            tmp1->right = tmp2;
                            tmp2->left = tmp1;
                            tmp1 = tmp2;
                            
                            ref_iter = ref_iter->right;
                        }
                        
                        ref_iter = node;
                        row_iter = row_iter->down;
                    }
                    
                } else {

                    node->up->down = first;
					first->up = node->up;
					last->down = node;
					node->up = last;

                    ModelNode* row_iter = first;
                    ModelNode* ref_iter = node;
                    ModelNode* tmp1 = 0;
                    ModelNode* tmp2 = 0;
                    
                    while(row_iter) {
                        
                        tmp1 = row_iter;
                        
                        while(ref_iter->right) {   // add one row
                            
                            tmp2 = new ModelNode;
                            tmp1->right = tmp2;
                            tmp2->left = tmp1;
                            tmp1 = tmp2;
                            
                            ref_iter = ref_iter->right;
                        }
                        
                        ref_iter = node;
                        row_iter = row_iter->down;
                    }
                    
                }

            } else {	// too large row given, append to tail

                assert(node);
                assert(node->down == 0);
                
                node->down = first;
				first->up = node;

                ModelNode* row_iter = first;
                ModelNode* ref_iter = node;
                ModelNode* tmp1 = 0;
                ModelNode* tmp2 = 0;

                while(row_iter) {
                    
                    tmp1 = row_iter;

                    while(ref_iter->right) {   // add one row
                        
                        tmp2 = new ModelNode;
                        tmp1->right = tmp2;
                        tmp2->left = tmp1;
                        tmp1 = tmp2;
                        
                        ref_iter = ref_iter->right;
                    }
                
                    ref_iter = node;
                    row_iter = row_iter->down;
                }
                
            }
		}

		return true;
	}

	bool AbstractListModel::RemoveRows (int row, int count,
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

			for(int i = 0; i < count; i++)
			{
				last = node->down;
				DestroyRow(node);
				node = last;

				if(node == 0)
					break;
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

	void AbstractListModel::ClearAllChildNodes()
	{
		ModelNode* node = root_->child;

		if(node) {

			ModelNode* next = 0;
			while(node){
				next = node->down;
				DestroyRow(node);
				node = next;
			}

			root_->child = 0;

		}
	}

}
