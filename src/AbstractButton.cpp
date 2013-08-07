/*
 * AbstractButton.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <BIL/AbstractButton.h>

namespace BIL {

	AbstractButton::AbstractButton (const wstring& text, Drawable *parent)
			: Controller(parent), _bg(RGBAf(0.75f, 0.75f, 0.75f, 1.0)),
			  _cornerRadius (1.0)
	{
		setPadding(Vec4i(2, 2, 2, 2));
		setText(text);
	}

	AbstractButton::~AbstractButton ()
	{
		// TODO Auto-generated destructor stub
	}

	void AbstractButton::setText (const wstring& text)
	{
		if(text.empty()) {
			// TODO: draw blank label
			return;
		}

		_text.append(text);

		calculateBox();

		_text.setOrigin(Coord3f(
								 _pos.coord.x + _padding.border.l,
								 _pos.coord.y + _padding.border.b,
								 0.0)
						 );
	}

	void AbstractButton::calculateBox(void)
	{
		Vec2ui box = _text.calculateBox();

		box.vec.x = box.vec.x + _padding.border.l + _padding.border.r;
		box.vec.y = box.vec.y + _padding.border.t + _padding.border.b;

		resize (box);
	}

	void AbstractButton::render (void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(_pos.coord.x,
					 _pos.coord.y,
					 _pos.coord.z);

		glColor4f(_bg.rgba.r, _bg.rgba.g, _bg.rgba.b, _bg.rgba.a);
		drawBox(GL_POLYGON,
					0.0f,
					0.0f,
					_size.vec.x,
					_size.vec.y,
					_cornerRadius);

		_text.render();

		glPopMatrix();
		glDisable(GL_BLEND);


	}

} /* namespace BIL */
