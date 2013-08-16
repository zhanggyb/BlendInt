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

#include <BIL/AbstractButton.hpp>

namespace BIL {

	AbstractButton::AbstractButton (Widget *parent)
		: Widget(parent), down_(false), checkable_(false), checked_(false),
		  background_(RGBAf(0.75f, 0.75f, 0.75f, 1.0)), corner_radius_ (1.0)
	{
		set_padding(Vec4i(2, 2, 2, 2));
	}

	AbstractButton::~AbstractButton ()
	{
		// TODO Auto-generated destructor stub
	}

	void AbstractButton::SetText (const wstring& text)
	{
		if(text.empty()) {
			// TODO: draw blank label
			return;
		}

		text_.Append(text);

		calculateBox();

		text_.set_origin(Coord3f(
								 pos_.coord.x + padding_.border.l,
								 pos_.coord.y + padding_.border.b,
								 0.0)
						 );
	}

	void AbstractButton::calculateBox(void)
	{
		Vec2ui box = text_.calculateBox();

		box.vec.x = box.vec.x + padding_.border.l + padding_.border.r;
		box.vec.y = box.vec.y + padding_.border.t + padding_.border.b;

		resize (box);
	}

} /* namespace BIL */
