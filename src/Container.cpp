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

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/Container.h>

namespace BIL {

	Container::Container (Traceable *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub
		size_ = Vec2ui(250, 200);
		setMargin(Vec4i(1, 1, 1, 1));
	}

	Container::~Container ()
	{
		// TODO Auto-generated destructor stub
	}

	void Container::render (void)
	{

	}

} /* namespace BIL */
