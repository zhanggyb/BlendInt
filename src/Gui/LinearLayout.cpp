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

#include <BlendInt/Gui/LinearLayout.hpp>

namespace BlendInt {

	LinearLayout::LinearLayout (AbstractContainer* container)
	: AbstractLayoutExt(container),
	  m_orientation(Horizontal),
	  m_alignment(AlignCenter),
	  m_space(2)
	{
	}

	LinearLayout::LinearLayout (AbstractContainer* container,
					Orientation orientation, int alignment, int space)
	: AbstractLayoutExt(container),
	  m_orientation(orientation),
	  m_alignment(alignment),
	  m_space(space)
	{
	}

	LinearLayout::~LinearLayout ()
	{
	}

	void LinearLayout::Fill ()
	{
	}

	void LinearLayout::Fill (const WidgetDeque* sub_widgets)
	{
		Distribute(sub_widgets);
		Align(sub_widgets);
	}
	
	void LinearLayout::Distribute (const WidgetDeque* sub_widgets)
	{
	}
	
	void LinearLayout::Align (const WidgetDeque* sub_widgets)
	{
	}

}
