/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BIL/Layout.hpp>

namespace BIL {

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent)
	{
		// TODO Auto-generated constructor stub

	}

	AbstractLayout::~AbstractLayout ()
	{
		// TODO Auto-generated destructor stub
	}

	bool AbstractLayout::addWidget (Widget* widget)
	{
		bool ret = addChild (widget);

		update();

		return ret;
	}

	bool AbstractLayout::addLayout(AbstractLayout* layout)
	{
		bool ret = addChild (layout);

		update();

		return ret;
	}

} /* namespace BIL */
