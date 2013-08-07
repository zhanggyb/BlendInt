/*
 * AbstractButton.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_ABSTRACTBUTTON_H_
#define _BIL_ABSTRACTBUTTON_H_

#include <string>

#include <BIL/Controller.h>
#include <BIL/Font.h>
#include <BIL/Tuple.h>
#include <BIL/TextBuffer.h>

using namespace std;

namespace BIL {

	class AbstractButton: public BIL::Controller
	{
	public:

		AbstractButton (const wstring& text, Drawable* parent = NULL);

		virtual ~AbstractButton ();

		void render (void);

		void setText (const wstring& text);

		void setFont (const Font& font);

		void setBackground (const RGBAf& color)
		{
			_bg = color;
		}

		void setCornerRadius (float rad)
		{
			_cornerRadius = rad;
		}

	private:	// member functions

		void calculateBox (void);

	private:	// member functions (disabled)

		AbstractButton (const AbstractButton& orig);
		AbstractButton& operator = (const AbstractButton& orig);

	private:

		TextBuffer _text;

		RGBAf _bg;

		float _cornerRadius;

	};

} /* namespace BIL */
#endif /* ABSTRACTBUTTON_H_ */
