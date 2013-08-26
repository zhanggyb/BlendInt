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

#include <BIL/Label.hpp>

namespace BIL {

	Label::Label (const wstring& text, Drawable *parent)
		: Widget (parent), background_(0x00000000)
	{
		set_padding(Tuple4i(2, 2, 2, 2));
		set_text(text);
	}

	Label::~Label ()
	{
		// TODO Auto-generated destructor stub
	}

	void Label::set_text (const wstring& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		text_.Append(label);

		calculateBox();

		text_.set_origin(Coord3f(
								 pos_.x() + padding_.border.l,
								 pos_.y() + padding_.border.b,
								 0.0)
						 );
	}

	void Label::calculateBox ()
	{
		Size box = text_.CalculateOutlineBoxSize();
		
		box.set_width(box.width() + padding_.border.l + padding_.border.r);
		box.set_height(box.height() + padding_.border.t + padding_.border.b);

		resize (box.width(), box.height());
	}

	void Label::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());
		glColor4ub(background_.r(), background_.g(),
				   background_.b(), background_.a());
		glRectf(0.0, 0.0, size_.width(), size_.height());

		text_.Render();

		glPopMatrix();
	}

	void Label::cursorPosEvent (double xpos, double ypos)
	{
		//cout << "Cursor Position: " << xpos << " " << ypos << endl;
	}
} /* namespace BIL */
