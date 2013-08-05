/*
 * Label.cpp
 *
 *  Created on: 2013年7月11日
 *      Author: zhanggyb
 */

#include <BIL/Label.h>

#include <BIL/Pen.h>

namespace BIL {

	Label::Label (const wstring& label, Drawable *parent)
		: Controller (parent), _label(label)
	{
		// TODO Auto-generated constructor stub
		setLabel (label);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(_pos.coord.x, _pos.coord.y, _pos.coord.z);

		glColor3f (1.0, 1.0, 1.0);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(_size.vec.x, 0.0, 0.0);
			glVertex3f(_size.vec.x, _size.vec.y, 0.0);
			glVertex3f(0.0, _size.vec.y, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		_label.render();

	}

} /* namespace BIL */
