/*
 * Label.cpp
 *
 *  Created on: 2013年7月11日
 *      Author: zhanggyb
 */

#include <BIL/Label.h>

#include <BIL/Pen.h>

namespace BIL {

	Label::Label (const wstring& text, Drawable *parent)
		: Controller (parent)
	{
		// TODO Auto-generated constructor stub
		setLabel (text);
	}

	Label::~Label ()
	{
		// TODO Auto-generated destructor stub
	}

	void Label::setLabel (const wstring& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		_label.append(label);

		Vec2ui box = _label.calculateBox();
		
		box.vec.x = box.vec.x + _padding.border.l + _padding.border.r;
		box.vec.y = box.vec.y + _padding.border.t + _padding.border.b;

		resize (box);
		_label.setOrigin(Coord3f(_pos.coord.x, _pos.coord.y + _size.vec.y, 0.0));
	}

	void Label::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();

		glTranslatef(_pos.coord.x, _pos.coord.y, _pos.coord.z);

		glColor4f(0.2f, 0.2f, 1.0f, 0.25f);
		glRectf(0.0, 0.0, _size.vec.x, _size.vec.y);

		_label.render();

		glPopMatrix();
	}

} /* namespace BIL */
