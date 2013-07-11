/*
 * Label.cpp
 *
 *  Created on: 2013年7月11日
 *      Author: zhanggyb
 */

#include <BIL/Label.h>

#include <BIL/GLDraw.h>

namespace BIL {

	Label::Label (BasicObject *parent)
	: Controller (parent)
	{
		// TODO Auto-generated constructor stub
		resize (128, _font.size() + 2);
	}

	Label::Label (const string& label, BasicObject *parent)
	: Controller (parent), _label(label)
	{
		// TODO Auto-generated constructor stub
		resize (128, _font.size() + 2);
	}

	Label::~Label ()
	{
		// TODO Auto-generated destructor stub
	}

	void Label::render(void)
	{
		GLDraw draw(this);
		draw.setColor(Color(0.5, 0.75, 0.65));
		draw.rect(_pos.getX(), _pos.getY(), getSize().getWidth(), getSize().getHeight());
	}

} /* namespace BIL */
