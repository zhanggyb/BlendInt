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

		
	}

	void Label::render(void)
	{

	}

} /* namespace BIL */
