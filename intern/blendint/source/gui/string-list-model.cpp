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

#include <gui/text.hpp>
#include <gui/string-list-model.hpp>

namespace BlendInt {

	StringListModel::StringListModel()
	: AbstractListModel(),
	  rows_(0)
	{

	}

	StringListModel::~StringListModel()
	{
		rows_ = 0;
	}

	void StringListModel::AddString (const String& string)
	{
		ModelIndex root = GetRootIndex();
		if(InsertRow(rows_, root)) {
			RefPtr<Text> data(new Text(string));
			ModelIndex index = GetIndex(rows_ - 1, 0, root);
			set_index_data(index, data);
		}
	}

	void StringListModel::InsertString (int row, const String& string)
	{
		ModelIndex root = GetRootIndex();
		if(InsertRow(row, root)) {
			RefPtr<Text> data(new Text(string));
			int valid_row = std::min(row, rows_ - 1);
			ModelIndex index = GetIndex(valid_row, 0, root);
			set_index_data(index, data);
		}
	}

	int StringListModel::GetRowCount (const ModelIndex& parent) const
	{
		return rows_;
	}

	int StringListModel::GetColumnCount (const ModelIndex& parent) const
	{
		return 1;
	}

	bool StringListModel::InsertColumns (int column, int count,
	        const ModelIndex& parent)
	{
		return false;
	}

	bool StringListModel::RemoveColumns (int column, int count,
	        const ModelIndex& parent)
	{
		return false;
	}

	bool StringListModel::InsertRows (int row, int count,
			const ModelIndex& parent)
	{
		bool retval = false;

		retval = AbstractListModel::InsertRows(row, count, parent);
		if (retval) {
			rows_ += count;
		}

		return retval;
	}

int StringListModel::GetPreferredColumnWidth (int index,
                                              const ModelIndex& parent) const
{
  // TODO: return a correct width
  return 80;
}

int StringListModel::GetPreferredRowHeight (int index,
                                            const ModelIndex& parent) const
{
  return BlendInt::Font::default_height();
}

	bool StringListModel::RemoveRows (int row, int count,
	        const ModelIndex& parent)
	{
		bool retval = false;

		retval = AbstractListModel::RemoveRows(row, count, parent);
		if(retval) {

			if((row + count) > rows_) {	// if count too large
				count = rows_ - row;
			}
			rows_ -= count;
		}

		return retval;
	}

#ifdef DEBUG

	void StringListModel::Print()
	{
		ModelNode* node = root()->child;

		Text* text = 0;
		int i = 0;
		while(node) {
			text = dynamic_cast<Text*>(node->data.get());
			if(text) {
				DBG_PRINT_MSG("node %d: %s", i, ConvertFromString(text->text()).c_str());
			}
			node = node->down;
			i++;
		}
	}

#endif

}
