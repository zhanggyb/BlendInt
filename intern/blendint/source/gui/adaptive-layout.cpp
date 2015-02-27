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

#include <gui/adaptive-layout.hpp>

namespace BlendInt {

	AdaptiveLayout::AdaptiveLayout(Orientation orient, int align, int space)
	: AbstractLayout(),
	  orientation_(orient),
	  alignment_(align),
	  space_(space)
	{
		set_size(200, 200);
	}

	AdaptiveLayout::AdaptiveLayout (int width, int height, const Margin& margin,
	        Orientation orient, int align, int space)

	: AbstractLayout(width, height, margin),
	  orientation_(orient),
	  alignment_(align),
	  space_(space)
	{
	}

	AdaptiveLayout::~AdaptiveLayout () {
	}

	bool AdaptiveLayout::AddWidget (AbstractWidget* widget)
	{
		return true;
	}

	bool AdaptiveLayout::InsertWidget (int index, AbstractWidget* widget)
	{
		return true;
	}

	bool AdaptiveLayout::InsertWidget (int row, int column,
	        AbstractWidget* widget)
	{
		return true;
	}

	void AdaptiveLayout::PerformMarginUpdate (const Margin& margin)
	{
	}

	bool AdaptiveLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool AdaptiveLayout::PositionUpdateTest (
	        const PositionUpdateRequest& request)
	{
		return true;
	}

	void AdaptiveLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
	}

}
