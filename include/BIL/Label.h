/*
 * Label.h
 *
 *	Created on: 2013年7月11日
 *		Author: zhanggyb
 */

#ifndef _BIL_LABEL_H_
#define _BIL_LABEL_H_

#include <string>

#include <BIL/Controller.h>
#include <BIL/TextBuffer.h>
#include <BIL/Font.h>

using namespace std;

namespace BIL {

	class Label: public BIL::Controller
	{
	public:

		Label (const wstring& label, Drawable * parent = NULL);

		virtual ~Label ();

		void setText (const wstring& label);

		void setFont (const Font& font)
		{
			_text.setFont(font);
			calculateBox();
		}

		void setTextColor (const RGBAf& fg,
				const RGBAf& bg = RGBAf(0.0, 0.0, 0.0, 0.0))
		{
			_text.setForeground(fg);
			_text.setBackground(bg);
		}

		void setBackground (const RGBAf& color)
		{
			_bg = color;
		}

		virtual void render (void);

	private:	// member functions

		void calculateBox (void);

	private:

		TextBuffer _text;

		/** Background color, default: transparent */
		RGBAf _bg;

	};

} /* namespace BIL */
#endif /* LABEL_H_ */
