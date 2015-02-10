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

#include <BlendInt/Gui/TableAdjustment.hpp>

namespace BlendInt {

	TableAdjustment::TableAdjustment(AbstractView* view, unsigned int row, unsigned int column, int space)
	: AbstractAdjustment(view),
	  row_(row),
	  column_(column),
	  space_(space)
	{
		assert(view->subs_count() == (int)(row * column));
	}

	TableAdjustment::~TableAdjustment ()
	{
	}

	void TableAdjustment::Adjust (int x, int y, int w, int h)
	{
	}

}
