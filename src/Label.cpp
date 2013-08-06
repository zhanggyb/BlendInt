/*
 * Label.cpp
 *
 *  Created on: 2013年7月11日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/Label.h>
#include <BIL/Pen.h>

namespace BIL {

	Label::Label (const wstring& text, Drawable *parent)
		: Controller (parent), _bg(RGBAf(0.0, 0.0, 0.0, 0.0))
	{
		// TODO Auto-generated constructor stub
		setPadding(Vec4i(2, 2, 2, 2));
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

		calculateBox();

		_label.setOrigin(Coord3f(
								 _pos.coord.x + _padding.border.l,
								 _pos.coord.y + _padding.border.b,
								 0.0)
						 );
	}

	void Label::calculateBox(void)
	{
		Vec2ui box = _label.calculateBox();
		
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

		_label.render();

		glPopMatrix();
	}

} /* namespace BIL */
