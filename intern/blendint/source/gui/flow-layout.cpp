/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <gui/flow-layout.hpp>

namespace BlendInt {

	FlowLayout::FlowLayout()
	: AbstractLayout()
	{

	}

	FlowLayout::~FlowLayout()
	{
	}

	bool FlowLayout::AddWidget(AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool FlowLayout::InsertWidget(int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool FlowLayout::InsertWidget(int row, int column, AbstractWidget* widget)
	{
		if(InsertSubView(column, widget)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool FlowLayout::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		return true;
	}

	bool FlowLayout::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		return true;
	}

}
