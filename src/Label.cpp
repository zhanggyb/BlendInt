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

#include <BIL/Label.h>
#include <BIL/Pen.h>

namespace BIL {

	Label::Label (const wstring& text, Drawable *parent)
		: Widget (parent), _bg(RGBAf(0.0, 0.0, 0.0, 0.0))
	{
		setPadding(Vec4i(2, 2, 2, 2));
		setText(text);
	}

	Label::~Label ()
	{
		// TODO Auto-generated destructor stub
	}

	void Label::setText (const wstring& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		_text.append(label);

		calculateBox();

		_text.setOrigin(Coord3f(
								 _pos.coord.x + _padding.border.l,
								 _pos.coord.y + _padding.border.b,
								 0.0)
						 );
	}

	void Label::calculateBox(void)
	{
		Vec2ui box = _text.calculateBox();
		
		box.vec.x = box.vec.x + _padding.border.l + _padding.border.r;
		box.vec.y = box.vec.y + _padding.border.t + _padding.border.b;

		resize (box);
	}

	void Label::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTranslatef(_pos.coord.x,
					 _pos.coord.y,
					 _pos.coord.z);

		glColor4f(_bg.rgba.r, _bg.rgba.g, _bg.rgba.b, _bg.rgba.a);
		glRectf(0.0, 0.0, _size.vec.x, _size.vec.y);

		_text.render();

		glPopMatrix();
	}

	void Label::cursorPosEvent (double xpos, double ypos)
	{
		//cout << "Cursor Position: " << xpos << " " << ypos << endl;
	}
} /* namespace BIL */
